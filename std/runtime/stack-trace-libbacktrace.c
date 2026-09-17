/* Symbol resolution for stack_trace_libbacktrace_rt.spice, backed by libbacktrace (deps/libbacktrace).
 *
 * libbacktrace reads the ELF/Mach-O/PE-COFF symbol table directly out of the executable file, so it resolves
 * every function that made it in, public or exported or not, with no '-rdynamic' involved. On ELF/Mach-O it
 * also reports each symbol's size, unlike dladdr()'s Dl_info, so a lookup past the end of its symbol can be
 * rejected instead of misattributed; COFF has no size field.
 *
 * The state is opened against the main executable (NULL filename), but on ELF/Mach-O backtrace_initialize()
 * also walks every shared library already loaded at the time of the first lookup (dl_iterate_phdr()/dyld image
 * APIs) and folds each into the same state, so libc/.so frames resolve too on those platforms. PE/COFF has no
 * such enumeration, so Windows only covers the main executable. A library dlopen()'d afterward, or one with no
 * symbol table, stays unresolved.
 *
 * Wrapped in a shim rather than called directly from Spice because backtrace_syminfo() takes a C callback: a
 * Spice function converted to a raw pointer is a '.fatthunk' with an extra leading captures pointer, not a bare
 * function pointer (spicelang/spice#1392), and 'ext' declarations can't express this API's struct/callback
 * shapes anyway.
 */

#include <stdint.h>
#include <stddef.h>

/* Mirrors the fields resolveFromLibbacktrace() needs: the symbol's mangled name, its start address and its
 * size. 'name' points into memory libbacktrace keeps mapped for the life of the process, so it stays valid
 * without being copied here - the caller copies it into a Spice String immediately after the call returns.
 */
struct SpiceSyminfoResult {
  const char *name;
  uintptr_t address;
  uintptr_t size;
};

/* SPICE_HAVE_LIBBACKTRACE is passed by ExternalLinkerInterface::prepare() only when a prebuilt archive exists
 * for the target (runtime/lib/<os>-<arch>/libbacktrace.a, see SystemUtil::findLibbacktraceStaticLib()). This
 * file compiles unconditionally whenever a program imports stack_trace_rt.spice, so without the define it must
 * not reference libbacktrace.h at all, or the link would fail with undefined symbols. Below this guard,
 * resolution just always reports "unresolved". */
#ifdef SPICE_HAVE_LIBBACKTRACE

#include <pthread.h>

/* Vendored copy of deps/libbacktrace/backtrace.h (license text intact): deps/ is build-time only, used to
 * produce runtime/lib/<os>-<arch>/libbacktrace.a, not something an installed Spice ships - so this shim's own
 * header has to live somewhere the std lib actually carries. */
#include "backtrace.h"

static struct backtrace_state *spiceBacktraceState;
static pthread_once_t spiceBacktraceInitOnce = PTHREAD_ONCE_INIT;

/* Errors are reported to the Spice caller as "unresolved" rather than surfaced here - a missing symbol table
 * or debug info is an ordinary, expected outcome (e.g. a stripped dependency), not a failure worth reporting
 * through a side channel the caller cannot see anyway. */
static void spiceBacktraceErrorCallback(void *data, const char *msg, int errnum) {
  (void)data;
  (void)msg;
  (void)errnum;
}

static void spiceBacktraceCreateState(void) {
  /* NULL asks libbacktrace to resolve the running executable's own path itself (e.g. via /proc/self/exe on
   * Linux, or GetModuleFileName() on Windows), rather than requiring argv[0]. Flag 1 sets THREADED, so
   * libbacktrace's own atomics make concurrent backtrace_syminfo() calls from multiple threads safe without a
   * lock on our side. */
  spiceBacktraceState = backtrace_create_state(NULL, 1, spiceBacktraceErrorCallback, NULL);
}

static void spiceBacktraceSyminfoCallback(void *data, uintptr_t pc, const char *symname, uintptr_t symval,
                                          uintptr_t symsize) {
  struct SpiceSyminfoResult *out = (struct SpiceSyminfoResult *)data;
  (void)pc;
  out->name = symname;
  out->address = symval;
  out->size = symsize;
}

/* Looks up the symbol containing 'addr' in the main executable's own symbol table.
 *
 * @param addr Address to resolve
 * @param out Filled with the result; out->name is NULL if the address could not be resolved
 * @return 1 if the lookup itself succeeded (a symbol table was available, even if 'addr' fell outside every
 *         known symbol, hence out->name may still be NULL), 0 if the lookup could not run at all
 */
int spiceBacktraceSyminfo(uintptr_t addr, struct SpiceSyminfoResult *out) {
  out->name = NULL;
  out->address = 0;
  out->size = 0;

  pthread_once(&spiceBacktraceInitOnce, spiceBacktraceCreateState);
  if (spiceBacktraceState == NULL)
    return 0;

  return backtrace_syminfo(spiceBacktraceState, addr, spiceBacktraceSyminfoCallback, spiceBacktraceErrorCallback, out);
}

#else

int spiceBacktraceSyminfo(uintptr_t addr, struct SpiceSyminfoResult *out) {
  (void)addr;
  out->name = NULL;
  out->address = 0;
  out->size = 0;
  return 0;
}

#endif
