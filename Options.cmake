# Spice version
set(SPICE_VERSION "dev" CACHE STRING "Spice build version")
add_definitions(-DSPICE_VERSION="${SPICE_VERSION}")
message(STATUS "Spice: Build version is set to '${SPICE_VERSION}'")

# Spice built by
set(SPICE_BUILT_BY "$ENV{USERNAME}" CACHE STRING "Spice built by person")
add_definitions(-DSPICE_BUILT_BY="${SPICE_BUILT_BY}")
message(STATUS "Spice: Built by is set to '${SPICE_BUILT_BY}'")

# Link time optimization
option(SPICE_LTO "Enable link time optimization (LTO) for the compiler executable" OFF)
if (SPICE_LTO)
    message(STATUS "Spice: LTO for Spice is enabled.")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -flto")
    SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto")
else()
    message(STATUS "Spice: LTO for Spice is disabled.")
endif()

# Developer build option (for build speed-ups)
option(SPICE_DEV_COMPILE "Developer-only option" OFF)
if(SPICE_DEV_COMPILE)
    message(STATUS "Spice: Dev build for Spice is enabled.")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ffunction-sections -fdata-sections")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
else()
    message(STATUS "Spice: Dev build for Spice is disabled.")
endif()

# Link statically
option(SPICE_LINK_STATIC "Link the compiler executable statically" OFF)
if (SPICE_LINK_STATIC)
    message(STATUS "Spice: Static linking for Spice is enabled.")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
else()
    message(STATUS "Spice: Static linking for Spice is disabled.")
endif()

# Coverage
option(SPICE_RUN_COVERAGE "Generate coverage reports for Spice source files" OFF)
if (SPICE_RUN_COVERAGE)
    message(STATUS "Spice: Coverage reports enabled.")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
else()
    message(STATUS "Spice: Coverage reports disabled.")
endif()

# ASAN
option(SPICE_RUN_ASAN "Run address sanitizer on executable" OFF)
if (SPICE_RUN_ASAN)
    message(STATUS "Spice: Address sanitizer enabled.")
    SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
else()
    message(STATUS "Spice: Address sanitizer disabled.")
endif()

# Valgrind testing
option(SPICE_RUN_VALGRIND_TESTING "Run Valgrind testing to detect memory leaks while running tests" OFF)
if (SPICE_RUN_VALGRIND_TESTING)
    message(STATUS "Spice: Valgrind testing enabled.")
    add_definitions(-DSPICE_RUN_VALGRIND_TESTING=true)
else()
    message(STATUS "Spice: Valgrind testing disabled.")
endif()



# Add additional definitions
IF (WIN32)
    ADD_DEFINITIONS(-DOS_WINDOWS)
ELSE()
    ADD_DEFINITIONS(-DOS_UNIX)
ENDIF()