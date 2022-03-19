# Spice version
set(SPICE_VERSION "dev" CACHE STRING "Spice build version")
add_definitions(-DSPICE_VERSION="${SPICE_VERSION}")
message(STATUS "Spice: Build version is set to '${SPICE_VERSION}'")

# Spice built by
set(SPICE_BUILT_BY "$ENV{USERNAME}" CACHE STRING "Spice built by person")
add_definitions(-DSPICE_BUILT_BY="${SPICE_BUILT_BY}")
message(STATUS "Spice: Built by is set to '${SPICE_BUILT_BY}'")

# Link statically
option(SPICE_LINK_STATIC "Link the compiler executable statically" OFF)
if (SPICE_LINK_STATIC)
    message(STATUS "Spice: Static linking for Spice is enabled.")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
else()
    message(STATUS "Spice: Static linking for Spice is disabled.")
endif()

# Coverage
option(SPICE_RUN_COVERAGE "Generate coverage reports for Spice source files" OFF)
if (SPICE_RUN_COVERAGE)
    message(STATUS "Spice: Coverage reports enabled.")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 --coverage -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS_DEBUG}")
else()
    message(STATUS "Spice: Coverage reports disabled.")
endif()

# GH Actions
option(SPICE_IS_GH_ACTIONS "Skip some tests, that do not work with GitHub actions" OFF)
if (SPICE_IS_GH_ACTIONS)
    message(STATUS "Spice: Skipping tests, that do not work with GH actions.")
    ADD_DEFINITIONS(-DSPICE_IS_GH_ACTIONS)
else()
    message(STATUS "Spice: Running all the tests.")
endif()