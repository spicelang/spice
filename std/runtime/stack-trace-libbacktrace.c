/* Symbol resolution for stack_trace_libbacktrace_rt.spice, backed by libbacktrace (deps/libbacktrace).
 *
 * libbacktrace reads the ELF/Mach-O/PE-COFF symbol table (and, on ELF/Mach-O, DWARF debug info if present,
 * which is not used here) directly from the executable file itself - not through dladdr()/.dynsym on POSIX, or
 * a DbgHelp-read PDB on Windows - so it resolves every function that made it into the symbol table, public or
 * not, exported or not, with no '-rdynamic' involved. On ELF/Mach-O it also reports each symbol's size, so a
 * lookup can tell "found, this far into the function" apart from "found nothing nearby", unlike dladdr()'s
 * Dl_info, which carries no size at all; COFF has no size field to report, so this is ELF/Mach-O only.
 *
 * The state is scoped to one file - the main executable, resolved by libbacktrace itself when passed a NULL
 * filename - so it does not see shared library frames (libc, etc.); those still go through the dladdr()-based
 * resolveSymbol() in stack_trace_symbol_rt.spice, which resolveFromLibbacktrace() falls back to.
 *
 * Wrapped in a shim rather than called directly from Spice for two reasons: backtrace_syminfo() takes a C
 * callback, and a Spice function converted to a raw pointer is not a bare function pointer - it becomes a
 * '.fatthunk' carrying an extra leading captures pointer, which shifts every argument
 * (spicelang/spice#1392) - and Spice's 'ext' declarations can only import functions, not the
 * struct-by-value/callback shapes this API uses.
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

/* SPICE_HAVE_LIBBACKTRACE is passed by ExternalLinkerInterface::prepare() only when it also puts
 * runtime/lib/<arch>-<os>/libbacktrace.a on the link line - i.e. only when a prebuilt archive exists for the
 * current target (see SystemUtil::findLibbacktraceStaticLib()). This file is compiled unconditionally
 * whenever a program imports stack_trace_rt.spice, regardless of that - so without the define, it must not
 * reference anything from libbacktrace.h at all, or the link would fail with undefined symbols wherever no
 * prebuilt archive is available (an unsupported host arch/os, or one CI/dev setup has not built one for yet).
 * Below this guard, resolution just always reports "unresolved", the same as if libbacktrace ran and found
 * nothing - callers fall through to the platform's own resolver either way. */
#ifdef SPICE_HAVE_LIBBACKTRACE

#include <pthread.h>

/* Vendored copy of deps/libbacktrace/backtrace.h, unmodified but for this comment, alongside its own license
 * text further up in that file. The compiler compiles this shim on demand from the std lib it ships - deps/ is
 * a build-time-only submodule used to produce runtime/lib/<arch>-<os>/libbacktrace.a (see dev-setup.py and
 * .github/workflows/publish.yml), not something an installed Spice ships - so the public header this shim needs
 * has to live somewhere the std lib actually carries. */
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
