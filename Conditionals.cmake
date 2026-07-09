include(CheckCXXCompilerFlag)

# Check if linker supports --gc-sections
set(CMAKE_REQUIRED_FLAGS "-Wl,--gc-sections")
check_cxx_compiler_flag("" LINKER_SUPPORTS_GC_SECTIONS)
set(GC_SECTIONS_FLAG "")
if (LINKER_SUPPORTS_GC_SECTIONS)
    set(GC_SECTIONS_FLAG "-Wl,-gc-sections")
endif()

# Check if linker supports full RELRO (-z relro -z now). ELF-specific, so only probe on Linux;
# macOS/Windows linkers don't share this semantics.
set(RELRO_NOW_FLAG "")
if (UNIX AND NOT APPLE)
    set(CMAKE_REQUIRED_FLAGS "-Wl,-z,relro,-z,now")
    check_cxx_compiler_flag("" LINKER_SUPPORTS_RELRO_NOW)
    if (LINKER_SUPPORTS_RELRO_NOW)
        set(RELRO_NOW_FLAG "-Wl,-z,relro,-z,now")
    endif()
    unset(CMAKE_REQUIRED_FLAGS)
endif()

# GCC 16's driver spec emits virtual library names like gcc_s_asneeded and
# atomic_asneeded. CMake captures these into CMAKE_<LANG>_IMPLICIT_LINK_LIBRARIES,
# and LLVM bakes them into its exported INTERFACE_LINK_LIBRARIES when it is built
# with GCC 16. They have no .a counterpart, so they break -static links. Strip
# them globally; the GCC driver re-emits the right form for the active mode.
foreach (_spice_lang IN ITEMS C CXX)
    if (DEFINED CMAKE_${_spice_lang}_IMPLICIT_LINK_LIBRARIES)
        list(REMOVE_ITEM CMAKE_${_spice_lang}_IMPLICIT_LINK_LIBRARIES
            gcc_s_asneeded
            atomic_asneeded)
    endif ()
endforeach ()

# Set release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdata-sections -ffunction-sections ${GC_SECTIONS_FLAG} -fvisibility=hidden")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${RELRO_NOW_FLAG}")

# Harden Linux release binaries: full RELRO (above) + fortify source. Requires -O1+, which the
# release flags above already provide. macOS/Windows toolchains don't share glibc's *_chk symbols.
if (UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -D_FORTIFY_SOURCE=2")
endif()

# On macOS, libc++ marks std::wstring_convert (used by CLI11) as deprecated. Keep it a warning, not an error.
set(SPICE_EXTRA_COMPILE_OPTIONS "")
if (APPLE)
    list(APPEND SPICE_EXTRA_COMPILE_OPTIONS -Wno-error=deprecated-declarations)
endif()
