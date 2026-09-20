# Build the vendored libbacktrace (deps/libbacktrace) as a static library.
#
# std/runtime/stack_trace_rt.spice links every Spice executable that captures a stack trace against libbacktrace,
# via its '-lbacktrace' linker-flag attribute. Relying on a copy installed on the user's machine is not an option:
# libbacktrace has no distro packages worth the name (GCC builds it as an internal support library and installs
# neither the archive nor <backtrace.h>), so the library is vendored as a submodule and built here. The result is
# written into the std tree, at std/runtime/lib/libbacktrace.a, next to the runtime sources that need it - that dir
# is what ExternalLinkerInterface adds to the linker's search path. Because it is a host binary, the release
# pipeline ships each platform's own copy rather than one built centrally; see 'Linking libbacktrace' in
# media/specs/stack-traces.md.
#
# Upstream builds with autotools. We do not run its ./configure, because that would need a POSIX shell on every
# platform Spice is built on and would sit outside the CMake dependency graph. Instead this file mirrors the
# handful of decisions configure makes - which object-format reader to compile, whether to use mmap, and the
# contents of config.h and backtrace-supported.h - using CMake's own probes. Only the library is built; upstream's
# test suite is not.

include(CheckCSourceCompiles)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)

get_filename_component(LIBBACKTRACE_DIR "${CMAKE_CURRENT_LIST_DIR}/../libbacktrace" ABSOLUTE)
if (NOT EXISTS ${LIBBACKTRACE_DIR}/backtrace.c)
    message(FATAL_ERROR "Spice: deps/libbacktrace is empty. Run 'python setup-deps.py' to check out the submodules.")
endif ()

########################## Probes (the ./configure equivalents) ##########################

# AC_USE_SYSTEM_EXTENSIONS. dl_iterate_phdr in particular is only declared with _GNU_SOURCE defined, so it has to
# be set for the probes below as well as for the library sources themselves.
set(LIBBACKTRACE_DEFINITIONS "")
if (UNIX)
    list(APPEND LIBBACKTRACE_DEFINITIONS _GNU_SOURCE)
    set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
endif ()

check_include_file(link.h HAVE_LINK_H)
check_include_file(sys/link.h HAVE_SYS_LINK_H)
check_include_file(sys/ldr.h HAVE_SYS_LDR_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(mach-o/dyld.h HAVE_MACH_O_DYLD_H)
check_include_file(windows.h HAVE_WINDOWS_H)
# tlhelp32.h is not self-contained; it only compiles after windows.h.
check_include_files("windows.h;tlhelp32.h" HAVE_TLHELP32_H)

check_symbol_exists(strnlen string.h HAVE_DECL_STRNLEN)
check_symbol_exists(getpagesize unistd.h HAVE_DECL_GETPAGESIZE)
check_symbol_exists(_pgmptr stdlib.h HAVE_DECL__PGMPTR)
check_symbol_exists(fcntl fcntl.h HAVE_FCNTL)
check_symbol_exists(lstat sys/stat.h HAVE_LSTAT)
check_symbol_exists(readlink unistd.h HAVE_READLINK)
check_symbol_exists(getexecname stdlib.h HAVE_GETEXECNAME)
check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
check_symbol_exists(mmap sys/mman.h LIBBACKTRACE_HAVE_MMAP)
if (HAVE_LINK_H)
    check_symbol_exists(dl_iterate_phdr link.h HAVE_DL_ITERATE_PHDR)
endif ()
if (HAVE_SYS_LDR_H)
    check_symbol_exists(loadquery sys/ldr.h HAVE_LOADQUERY)
endif ()

check_c_source_compiles("
    #include <sys/types.h>
    #include <sys/sysctl.h>
    int main(void) { int mib0 = CTL_KERN, mib1 = KERN_PROC, mib2 = KERN_PROC_PATHNAME; return mib0 + mib1 + mib2; }
" HAVE_KERN_PROC)
check_c_source_compiles("
    #include <sys/types.h>
    #include <sys/sysctl.h>
    int main(void) { int mib0 = CTL_KERN, mib1 = KERN_PROC_ARGS, mib2 = KERN_PROC_PATHNAME; return mib0 + mib1 + mib2; }
" HAVE_KERN_PROC_ARGS)

check_c_source_compiles("
    int main(void) { int i = 0; __atomic_store_n(&i, 1, __ATOMIC_RELEASE); return __atomic_load_n(&i, __ATOMIC_ACQUIRE); }
" HAVE_ATOMIC_FUNCTIONS)
check_c_source_compiles("
    int main(void) { int i = 0; __sync_bool_compare_and_swap(&i, i, i); __sync_lock_test_and_set(&i, 1);
                     __sync_lock_release(&i); return i; }
" HAVE_SYNC_FUNCTIONS)

# The unwinder the capture half of the library is built on. Without it only the stub in nounwind.c can be built,
# and backtrace_full() reports that backtraces are unsupported rather than failing to link.
check_c_source_compiles("
    #include <unwind.h>
    int main(void) { _Unwind_Backtrace(0, 0); return 0; }
" LIBBACKTRACE_HAVE_UNWIND_BACKTRACE)
check_c_source_compiles("
    #include <unwind.h>
    int main(void) { int ip_before; _Unwind_GetIPInfo(0, &ip_before); return ip_before; }
" HAVE_GETIPINFO)

unset(CMAKE_REQUIRED_DEFINITIONS)

########################## Source selection (configure's *_FILE variables) ##########################

# Object-format reader. configure picks this by running filetype.awk over a compiled object file; the host
# platform determines it just as well, and without needing awk.
set(BACKTRACE_ELF_SIZE unused)
set(BACKTRACE_XCOFF_SIZE unused)
set(BACKTRACE_SUPPORTED 1)
set(BACKTRACE_SUPPORTS_DATA 1)
math(EXPR LIBBACKTRACE_WORD_SIZE "${CMAKE_SIZEOF_VOID_P} * 8")
if (APPLE)
    set(LIBBACKTRACE_FORMAT_FILE macho.c)
elseif (WIN32)
    set(LIBBACKTRACE_FORMAT_FILE pecoff.c)
    set(BACKTRACE_SUPPORTS_DATA 0)
elseif (CMAKE_SYSTEM_NAME STREQUAL "AIX")
    set(LIBBACKTRACE_FORMAT_FILE xcoff.c)
    set(BACKTRACE_XCOFF_SIZE ${LIBBACKTRACE_WORD_SIZE})
    set(BACKTRACE_SUPPORTS_DATA 0)
elseif (UNIX)
    set(LIBBACKTRACE_FORMAT_FILE elf.c)
    set(BACKTRACE_ELF_SIZE ${LIBBACKTRACE_WORD_SIZE})
else ()
    message(WARNING "Spice: unknown object file format for '${CMAKE_SYSTEM_NAME}'; Spice stack traces will be empty")
    set(LIBBACKTRACE_FORMAT_FILE unknown.c)
    set(BACKTRACE_SUPPORTED 0)
endif ()

# Stack walker.
if (LIBBACKTRACE_HAVE_UNWIND_BACKTRACE)
    set(LIBBACKTRACE_BACKTRACE_FILES backtrace.c simple.c)
else ()
    message(WARNING "Spice: no _Unwind_Backtrace on this platform; Spice stack traces will be empty")
    set(LIBBACKTRACE_BACKTRACE_FILES nounwind.c)
    set(BACKTRACE_SUPPORTED 0)
endif ()

# File reader and allocator. mmap lets the library run from a signal handler, so it is preferred wherever the
# platform has it along with anonymous mappings.
set(BACKTRACE_USES_MALLOC 1)
if (LIBBACKTRACE_HAVE_MMAP)
    set(LIBBACKTRACE_VIEW_FILE mmapio.c)
    check_c_source_compiles("
        #include <sys/mman.h>
        #if !defined(MAP_ANONYMOUS) && !defined(MAP_ANON)
        #error no MAP_ANONYMOUS
        #endif
        int main(void) { return 0; }
    " LIBBACKTRACE_HAVE_MAP_ANONYMOUS)
    if (LIBBACKTRACE_HAVE_MAP_ANONYMOUS)
        set(LIBBACKTRACE_ALLOC_FILE mmap.c)
        set(BACKTRACE_USES_MALLOC 0)
    else ()
        set(LIBBACKTRACE_ALLOC_FILE alloc.c)
    endif ()
else ()
    set(LIBBACKTRACE_VIEW_FILE read.c)
    set(LIBBACKTRACE_ALLOC_FILE alloc.c)
endif ()

if (HAVE_SYNC_FUNCTIONS)
    set(BACKTRACE_SUPPORTS_THREADS 1)
else ()
    set(BACKTRACE_SUPPORTS_THREADS 0)
endif ()

########################## Generated headers ##########################

# Both land in the build tree rather than in the submodule, so the checkout of deps/libbacktrace stays pristine.
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
configure_file(${LIBBACKTRACE_DIR}/backtrace-supported.h.in ${CMAKE_CURRENT_BINARY_DIR}/backtrace-supported.h @ONLY)

########################## The library ##########################

set(LIBBACKTRACE_SOURCES atomic.c dwarf.c fileline.c posix.c print.c sort.c state.c)
list(APPEND LIBBACKTRACE_SOURCES
    ${LIBBACKTRACE_BACKTRACE_FILES} ${LIBBACKTRACE_FORMAT_FILE} ${LIBBACKTRACE_VIEW_FILE} ${LIBBACKTRACE_ALLOC_FILE})
list(TRANSFORM LIBBACKTRACE_SOURCES PREPEND ${LIBBACKTRACE_DIR}/)

add_library(backtrace STATIC ${LIBBACKTRACE_SOURCES})
target_include_directories(backtrace SYSTEM PUBLIC ${LIBBACKTRACE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_compile_definitions(backtrace PRIVATE ${LIBBACKTRACE_DEFINITIONS})
# The archive is linked into Spice programs, which may themselves be shared libraries ('--output-container=shared'),
# so every object in it has to be position-independent.
set_target_properties(backtrace PROPERTIES POSITION_INDEPENDENT_CODE ON)
# Third-party code: build it the way upstream does and do not hold it to Spice's own warning settings.
target_compile_options(backtrace PRIVATE -funwind-tables -w)

# Emit the archive straight into the std tree, under the exact name '-lbacktrace' resolves to. Multi-config
# generators would otherwise append a per-config subdirectory, so pin the per-config properties as well.
get_filename_component(LIBBACKTRACE_OUTPUT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../std/runtime/lib" ABSOLUTE)
set_target_properties(backtrace PROPERTIES PREFIX "lib" SUFFIX ".a" ARCHIVE_OUTPUT_DIRECTORY ${LIBBACKTRACE_OUTPUT_DIR})
foreach (config IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${config} configUpper)
    set_target_properties(backtrace PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${configUpper} ${LIBBACKTRACE_OUTPUT_DIR})
endforeach ()

message(STATUS "Spice: Building vendored libbacktrace (${LIBBACKTRACE_FORMAT_FILE}) to ${LIBBACKTRACE_OUTPUT_DIR}")
