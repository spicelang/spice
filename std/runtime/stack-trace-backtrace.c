/* Stack trace capture and symbolization for stack_trace_rt.spice, on top of libbacktrace.
 *
 * libbacktrace does both halves of the job in one library, and the same way on every platform it supports: it
 * unwinds through the unwind tables that IRGenerator's 'uwtable' attribute always emits (so no frame pointers
 * are needed and any optimization level works), and it resolves each program counter against the executable's
 * own debug info and symbol table - DWARF in ELF and Mach-O, PDB/COFF in PE. That is why it replaced the
 * hand-written combination of _Unwind_Backtrace and dladdr()/DbgHelp that used to live here: those needed one
 * implementation per platform and resolved less.
 *
 * Written in C because every libbacktrace entry point takes a callback, and a Spice function converted to a raw
 * pointer is not a bare function pointer: it becomes a '.fatthunk' carrying an extra leading capture-struct
 * pointer, which shifts every argument (spicelang/spice#1392). A C callback has no such problem.
 *
 * libbacktrace's own <backtrace.h> is not installed by the toolchains that ship the library (GCC builds it as an
 * internal support library), so the handful of declarations this file needs are mirrored below instead. They are
 * the stable public API and have not changed since the library was first released.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>   /* for toSymbolTableAddress() below, which reads the module's own file */
#include <windows.h>
#endif

/* ==================== libbacktrace API (mirrored from <backtrace.h>) ==================== */

struct backtrace_state;

typedef void (*backtrace_error_callback)(void *data, const char *msg, int errnum);
typedef int (*backtrace_full_callback)(void *data, uintptr_t pc, const char *filename, int lineno,
                                       const char *function);
typedef int (*backtrace_simple_callback)(void *data, uintptr_t pc);
typedef void (*backtrace_syminfo_callback)(void *data, uintptr_t pc, const char *symname, uintptr_t symval,
                                           uintptr_t symsize);

extern struct backtrace_state *backtrace_create_state(const char *filename, int threaded,
                                                      backtrace_error_callback error_callback, void *data);
extern int backtrace_full(struct backtrace_state *state, int skip, backtrace_full_callback callback,
                          backtrace_error_callback error_callback, void *data);
extern int backtrace_simple(struct backtrace_state *state, int skip, backtrace_simple_callback callback,
                            backtrace_error_callback error_callback, void *data);
extern int backtrace_syminfo(struct backtrace_state *state, uintptr_t addr, backtrace_syminfo_callback callback,
                             backtrace_error_callback error_callback, void *data);

/* ==================== Shared with Spice ==================== */

/* One captured frame. Mirrors 'NativeStackFrame' in stack_trace_rt.spice, field for field: 'offset' is fixed at
 * 64 bit because Spice's 'unsigned long' always is, unlike C's 'unsigned long' on Windows. The two names are
 * heap-allocated copies owned by the caller - see spiceReleaseStackTrace(). */
typedef struct SpiceStackFrame {
  void *address;
  uint64_t offset;
  char *functionName;
  char *fileName;
  int32_t lineNumber;
} SpiceStackFrame;

/* ==================== Implementation ==================== */

/* Moves an address into the coordinate space libbacktrace's PE symbol table uses, on the one platform where the
 * two differ.
 *
 * libbacktrace builds that table at the image base recorded in the module's file on disk - the address the module
 * asks to be loaded at - but Windows puts modules somewhere else whenever ASLR is on, which it is by default. Its
 * DWARF reader accounts for the difference; the symbol table reader is handed the unrelocated base and does not,
 * so every address in a running program sorts past the last entry in the table and each frame comes back named
 * after it. Until that is fixed upstream, the shift is undone here, by subtracting the distance the module
 * carrying the address was actually relocated by.
 *
 * Only the lookup sees the adjusted address - the frame keeps the real one. The name and symbol value that come
 * back are the right ones, and the offset is a difference of two addresses within the table's space, so the shift
 * cancels out of it. An address whose module cannot be read is left alone, resolving no worse than before. */
#if defined(_WIN32)

/* Reads a module's preferred base out of its file.
 *
 * It has to come from the file: when the loader relocates a module it rewrites OptionalHeader.ImageBase in the
 * mapped copy to the address it actually used, so the header in memory reports the module as unmoved however far
 * it travelled. libbacktrace parses the file, so the file is where the coordinate space of its symbol table comes
 * from, and reading the mapped copy instead just yields a shift of zero. */
static int readPreferredBase(HMODULE module, uintptr_t *preferredBase) {
  char modulePath[MAX_PATH];
  const DWORD length = GetModuleFileNameA(module, modulePath, (DWORD)sizeof(modulePath));
  if (length == 0 || length >= (DWORD)sizeof(modulePath))
    return 0;

  FILE *moduleFile = fopen(modulePath, "rb");
  if (moduleFile == NULL)
    return 0;

  IMAGE_DOS_HEADER dosHeader;
  IMAGE_NT_HEADERS ntHeaders;
  const int read = fread(&dosHeader, sizeof dosHeader, 1, moduleFile) == 1 &&
                   dosHeader.e_magic == IMAGE_DOS_SIGNATURE && dosHeader.e_lfanew >= 0 &&
                   fseek(moduleFile, dosHeader.e_lfanew, SEEK_SET) == 0 &&
                   fread(&ntHeaders, sizeof ntHeaders, 1, moduleFile) == 1 &&
                   ntHeaders.Signature == IMAGE_NT_SIGNATURE;
  fclose(moduleFile);
  if (!read)
    return 0;

  *preferredBase = (uintptr_t)ntHeaders.OptionalHeader.ImageBase;
  return 1;
}

/* The executable's own shift, which nearly every frame of a trace needs, kept so that walking a stack does not
 * reopen the same file once per frame. Published as two atomics rather than one pair, so a reader either sees a
 * value that was fully written or decides it is not there yet; two threads racing here both compute the same
 * number anyway. Frames in DLLs are rarer and read their module's file each time. */
static uintptr_t executableShift = 0;
static int executableShiftKnown = 0;

static int shiftOfModule(HMODULE module, uintptr_t *shift) {
  const int isExecutable = module == GetModuleHandleW(NULL);
  if (isExecutable && __atomic_load_n(&executableShiftKnown, __ATOMIC_ACQUIRE)) {
    *shift = __atomic_load_n(&executableShift, __ATOMIC_RELAXED);
    return 1;
  }

  uintptr_t preferredBase;
  if (!readPreferredBase(module, &preferredBase))
    return 0;
  *shift = (uintptr_t)module - preferredBase;

  if (isExecutable) {
    __atomic_store_n(&executableShift, *shift, __ATOMIC_RELAXED);
    __atomic_store_n(&executableShiftKnown, 1, __ATOMIC_RELEASE);
  }
  return 1;
}

static uintptr_t toSymbolTableAddress(uintptr_t pc) {
  /* UNCHANGED_REFCOUNT, so the handle needs no release; FROM_ADDRESS takes the address cast to a string. */
  HMODULE module = NULL;
  if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          (LPCWSTR)pc, &module) ||
      module == NULL)
    return pc;

  uintptr_t shift;
  if (!shiftOfModule(module, &shift))
    return pc;
  return pc - shift;
}
#else
/* Everywhere else libbacktrace resolves symbols against correctly biased addresses already. */
static uintptr_t toSymbolTableAddress(uintptr_t pc) { return pc; }
#endif

/* State passed through the libbacktrace callbacks of one capture. */
struct SpiceCaptureState {
  struct backtrace_state *state;
  SpiceStackFrame *frames;
  int capacity;
  int count;
  int skipEntries;
};

/* Every error libbacktrace reports is a missing-information error ("no debug info", "no symbol table", ...), and
 * every one of them is already visible in the result as an unresolved frame. Printing them would turn a stack
 * trace into a diagnostic about the stack trace, so they are dropped. */
static void ignoreError(void *data, const char *msg, int errnum) {
  (void)data;
  (void)msg;
  (void)errnum;
}

/* The one backtrace_state of this process. libbacktrace documents that it must be created at most once and can
 * never be freed, since it caches the parsed debug info that makes every later lookup cheap. */
static void *backtraceState = NULL;

/* Returns the process-wide state, creating it on first use, or NULL if it could not be created. */
static struct backtrace_state *obtainState(void) {
  struct backtrace_state *existing = (struct backtrace_state *)__atomic_load_n(&backtraceState, __ATOMIC_ACQUIRE);
  if (existing != NULL)
    return existing;

  /* Created with the threaded flag, so libbacktrace guards its own caches: several threads may dump a trace at
   * once. Passing NULL for the file name lets it find the running executable the platform's own way. */
  struct backtrace_state *created = backtrace_create_state(NULL, 1, ignoreError, NULL);
  if (created == NULL)
    return NULL;

  /* Two threads reaching this point together both get a valid state; the loser's is dropped rather than freed,
   * which is all that can be done with a state - and it happens at most once per process.
   *
   * Claiming the right to create one first, so that only a single state is ever made, would be tidier but worse
   * here: the threads that lost the claim would have to wait for the winner to publish, and a trace is often
   * dumped from a crash path, where the thread that would publish may be the one that just died. A bounded,
   * once-per-process memory cost in a rare race beats a stack trace that hangs instead of printing. */
  void *expected = NULL;
  if (!__atomic_compare_exchange_n(&backtraceState, &expected, (void *)created, 0, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE))
    return (struct backtrace_state *)expected;
  return created;
}

/* Copies a string libbacktrace handed out. The originals are documented as possibly becoming invalid once the
 * callback returns, so nothing may be kept by pointer. Returns NULL for NULL input and on allocation failure -
 * an unresolved frame is a better outcome than a failed stack trace. */
static char *duplicateString(const char *text) {
  if (text == NULL)
    return NULL;
  const size_t size = strlen(text) + 1;
  char *copy = (char *)malloc(size);
  if (copy != NULL)
    memcpy(copy, text, size);
  return copy;
}

/* Receives the symbol table entry containing one frame's address: the offset into the enclosing function, and a
 * name in case there was no debug info to take one from. */
static void collectSymbol(void *data, uintptr_t pc, const char *symname, uintptr_t symval, uintptr_t symsize) {
  (void)symsize;
  SpiceStackFrame *frame = (SpiceStackFrame *)data;
  if (symname == NULL) /* symbol table present, but this address is not in it */
    return;
  if (frame->functionName == NULL)
    frame->functionName = duplicateString(symname);
  /* An offset only means anything next to the name it counts from, and the copy above can fail, so the two are
   * filled in together or not at all - which is the invariant the capture test asserts. */
  if (frame->functionName != NULL && symval != 0 && pc >= symval)
    frame->offset = (uint64_t)(pc - symval);
}

/* Fills the next frame slot from an address, and returns it for the symbol lookup to complete. NULL once the
 * buffer is full. */
static SpiceStackFrame *beginFrame(struct SpiceCaptureState *capture, uintptr_t pc) {
  if (capture->count >= capture->capacity)
    return NULL;
  SpiceStackFrame *frame = &capture->frames[capture->count];
  frame->address = (void *)pc;
  frame->offset = 0;
  frame->functionName = NULL;
  frame->fileName = NULL;
  frame->lineNumber = 0;
  return frame;
}

/* Marks the end of the stack. The unwinder reports a zero program counter once it walks off the bottom, which
 * libbacktrace decrements to an all-ones address before handing it on; neither is a real frame. */
static int isEndOfStack(uintptr_t pc) { return pc == 0 || pc == UINTPTR_MAX; }

/* Takes one off the caller's skip budget, and says whether this entry is one of the ones being dropped.
 *
 * This is deliberately not folded into the 'skip' that backtrace_full() takes: that one counts physical frames
 * and is applied before symbolization, so with debug info a single skipped frame can swallow several entries -
 * one per function inlined into it. The caller counts in entries, the same units it gets back. */
static int isSkippedEntry(struct SpiceCaptureState *capture) {
  if (capture->skipEntries <= 0)
    return 0;
  capture->skipEntries--;
  return 1;
}

/* Invoked once per frame by backtrace_full(), innermost first, and once more per function inlined into it. */
static int collectFullFrame(void *data, uintptr_t pc, const char *filename, int lineno, const char *function) {
  struct SpiceCaptureState *capture = (struct SpiceCaptureState *)data;
  if (isEndOfStack(pc))
    return 1;
  if (isSkippedEntry(capture))
    return 0;

  SpiceStackFrame *frame = beginFrame(capture, pc);
  if (frame == NULL)
    return 1; /* buffer full - stop rather than drop the frames nearest the capture point */
  frame->functionName = duplicateString(function);
  frame->fileName = duplicateString(filename);
  if (frame->fileName != NULL) /* same as above: a line number without its file name says nothing */
    frame->lineNumber = lineno;
  /* backtrace_full() reports the name but not where the function starts, so the offset takes a second lookup.
   * For a frame that was inlined into another, this is the offset into the function it was inlined into. */
  backtrace_syminfo(capture->state, toSymbolTableAddress(pc), collectSymbol, ignoreError, frame);

  capture->count++;
  return 0;
}

/* Invoked once per frame by the backtrace_simple() fallback below, which reports no names at all. */
static int collectSimpleFrame(void *data, uintptr_t pc) {
  struct SpiceCaptureState *capture = (struct SpiceCaptureState *)data;
  if (isEndOfStack(pc))
    return 1;
  if (isSkippedEntry(capture))
    return 0;

  SpiceStackFrame *frame = beginFrame(capture, pc);
  if (frame == NULL)
    return 1;
  backtrace_syminfo(capture->state, toSymbolTableAddress(pc), collectSymbol, ignoreError, frame);

  capture->count++;
  return 0;
}

/* Fills 'frames' with up to 'capacity' frames of the calling stack, most recent call first, and returns how many
 * were written.
 *
 * The two skips count different things. 'skipNativeFrames' drops whole physical frames before anything is looked
 * up, starting with this function's own, and is for the stack trace runtime's own frames, which are always
 * physical. 'skipEntries' drops that many of the entries that would otherwise be written, and is for the caller,
 * who counts in the entries it gets back - not the same thing once debug info turns one physical frame into
 * several entries.
 *
 * Names and file names are heap-allocated; release them with spiceReleaseStackTrace() once they are copied out.
 */
int spiceCaptureStackTrace(SpiceStackFrame *frames, int capacity, int skipNativeFrames, int skipEntries) {
  if (frames == NULL || capacity <= 0 || skipNativeFrames < 0 || skipEntries < 0)
    return 0;
  struct backtrace_state *state = obtainState();
  if (state == NULL)
    return 0;

  struct SpiceCaptureState capture;
  capture.state = state;
  capture.frames = frames;
  capture.capacity = capacity;
  capture.count = 0;
  capture.skipEntries = skipEntries;

  /* A skip of 0 makes backtrace_full() start at its own caller, which is this function - exactly the frame
   * numbering documented above, so 'skipNativeFrames' passes straight through. It resolves as much as the
   * executable carries: function, file and line from debug info, function name alone from the symbol table, and
   * the bare address when there is neither. */
  backtrace_full(state, skipNativeFrames, collectFullFrame, ignoreError, &capture);

  /* It gives up entirely, without reporting a single frame, when it cannot even open the executable. Unwinding
   * itself still works in that case, so fall back to the addresses alone rather than to no trace at all. The
   * skip budget is reset, since nothing was written and the callback would otherwise have nothing left to drop. */
  if (capture.count == 0) {
    capture.skipEntries = skipEntries;
    backtrace_simple(state, skipNativeFrames, collectSimpleFrame, ignoreError, &capture);
  }

  return capture.count;
}

/* Frees the names of the first 'count' frames and nils them out, so releasing twice is harmless. */
void spiceReleaseStackTrace(SpiceStackFrame *frames, int count) {
  if (frames == NULL)
    return;
  for (int i = 0; i < count; i++) {
    free(frames[i].functionName);
    frames[i].functionName = NULL;
    free(frames[i].fileName);
    frames[i].fileName = NULL;
  }
}
