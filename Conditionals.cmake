include(CheckCXXCompilerFlag)

# Check if linker supports --gc-sections
set(CMAKE_REQUIRED_FLAGS "-Wl,--gc-sections")
check_cxx_compiler_flag("" LINKER_SUPPORTS_GC_SECTIONS)
set(GC_SECTIONS_FLAG "")
if (LINKER_SUPPORTS_GC_SECTIONS)
    set(GC_SECTIONS_FLAG "-Wl,-gc-sections")
endif()

# Set release flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fdata-sections -ffunction-sections ${GC_SECTIONS_FLAG} -fvisibility=hidden")
