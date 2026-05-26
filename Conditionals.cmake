include(CheckCXXCompilerFlag)

# Check if linker supports --gc-sections
set(CMAKE_REQUIRED_FLAGS "-Wl,--gc-sections")
check_cxx_compiler_flag("" LINKER_SUPPORTS_GC_SECTIONS)
set(GC_SECTIONS_FLAG "")
if (LINKER_SUPPORTS_GC_SECTIONS)
    set(GC_SECTIONS_FLAG "-Wl,-gc-sections")
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

# On macOS, libc++ marks std::wstring_convert (used by CLI11) as deprecated. Keep it a warning, not an error.
set(SPICE_EXTRA_COMPILE_OPTIONS "")
if (APPLE)
    list(APPEND SPICE_EXTRA_COMPILE_OPTIONS -Wno-error=deprecated-declarations)
endif()
