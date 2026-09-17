/* Boundary lookup for the compiler-emitted symbol table (IRGenerator::generateSymbolTable(), behind
 * '--keep-symbol-table'), backing resolveFromSymtab() in stack_trace_symtab_rt.spice.
 *
 * Every Spice object file that opts in contributes its own array of { address, name } entries to a section
 * named for its target's object format. This shim exposes the address range that section ends up occupying in
 * the linked binary, via spiceSymtabRange() - a plain function, since Spice's 'ext' declarations can only
 * import functions, not external data symbols.
 *
 * The three techniques below are exactly what compiler-rt's own profiling runtime uses for the same problem:
 * finding a metadata section the frontend scattered across many object files, at a known address range, without
 * a linker script.
 */

#include <stddef.h>

#if defined(__APPLE__)

/* ld64 synthesizes 'section$start$<segment>$<section>'/'section$end$...' only once some input file actually
 * contributes to that section - so a build with the flag off, where no Spice object defines '__spice_symtab' at
 * all, would leave these undefined. 'weak_import' is the static-link counterpart of ELF's weak-undefined: an
 * unresolved weak reference is left as NULL instead of failing the link, so this file links unconditionally.
 * The '$' in the real symbol name makes it unusable as a plain C identifier, so the local name is bound to it
 * via an asm label instead - the same idiom Apple's own headers use for this.
 */
extern const void *spiceSymtabRangeStart __asm("section$start$__DATA$__spice_symtab") __attribute__((weak_import));
extern const void *spiceSymtabRangeEnd __asm("section$end$__DATA$__spice_symtab") __attribute__((weak_import));

void spiceSymtabRange(const void **outBegin, const void **outEnd) {
  *outBegin = &spiceSymtabRangeStart;
  *outEnd = &spiceSymtabRangeEnd;
}

#elif defined(_WIN32)

/* The COFF linker concatenates same-stemmed sections in suffix order ('$a' < '$m' < '$z'), the same trick
 * MSVC's CRT uses for its init-function arrays. IRGenerator places its entries in '.spicesym$m'; the markers
 * here bracket that group regardless of whether anything landed in '$m'. They are typed and aligned exactly
 * like a real entry (two pointers), so no padding can separate a marker from the real entries next to it.
 */
struct SpiceSymtabEntry {
  const void *address;
  const char *name;
};

#pragma section(".spicesym$a", read)
#pragma section(".spicesym$z", read)
__declspec(allocate(".spicesym$a")) static const struct SpiceSymtabEntry spiceSymtabBegin = {NULL, NULL};
__declspec(allocate(".spicesym$z")) static const struct SpiceSymtabEntry spiceSymtabEndMarker = {NULL, NULL};

void spiceSymtabRange(const void **outBegin, const void **outEnd) {
  /* The first real entry, if there is one, starts right after the begin marker. */
  *outBegin = (const void *)(&spiceSymtabBegin + 1);
  *outEnd = (const void *)&spiceSymtabEndMarker;
}

#else

/* GNU ld and lld synthesize '__start_<name>'/'__stop_<name>' for any section whose name is a valid C
 * identifier, but only once some input file actually defines that section - so a build with the flag off,
 * where no Spice object contributes 'spice_symtab' at all, would leave these undefined. 'weak' makes an
 * unresolved reference fold to NULL instead of failing the link, so this file links unconditionally.
 */
extern const char __start_spice_symtab[] __attribute__((weak));
extern const char __stop_spice_symtab[] __attribute__((weak));

void spiceSymtabRange(const void **outBegin, const void **outEnd) {
  *outBegin = __start_spice_symtab;
  *outEnd = __stop_spice_symtab;
}

#endif
