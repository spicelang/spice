# Initialize GTest (git submodule in deps/)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/googletest ${CMAKE_BINARY_DIR}/deps/googletest EXCLUDE_FROM_ALL)

################ spicetest executable ################

set(SOURCES
        # Test fixture
        main.cpp
        TestRunner.cpp
        driver/TestDriver.cpp
        util/TestUtil.cpp
        util/BootstrapUtil.cpp
        # Unit tests
        unittest/UnitBlockAllocator.cpp
        unittest/UnitCommonUtil.cpp
        unittest/UnitCompileCache.cpp
        unittest/UnitFileUtil.cpp
        unittest/UnitSystemUtil.cpp
        unittest/UnitDriver.cpp
        unittest/UnitThreadPool.cpp
)

add_executable(spicetest ${SOURCES})
# Link with spicecore, gtest, gmock and llvm
target_link_libraries(spicetest PRIVATE spicecore GTest::gtest GTest::gmock ${LLVM_LIBS})
if (SPICE_ENABLE_TPDE)
    target_link_libraries(spicetest PRIVATE spice_tpde)
    # The std TPDE bindings (std/bindings/tpde) link against the TPDE static libraries and compile their C API wrapper
    # against the TPDE headers. spicetest exports the flags for both as TPDE_FLAGS (see main.cpp).
    set(TPDE_FLAGS "-I${CMAKE_SOURCE_DIR}/deps/tpde/tpde-llvm/include")
    foreach (TPDE_LIB_TARGET tpde_llvm tpde fadec disarm64 spdlog)
        if (TARGET ${TPDE_LIB_TARGET})
            get_target_property(TPDE_LIB_TYPE ${TPDE_LIB_TARGET} TYPE)
            if (TPDE_LIB_TYPE STREQUAL "STATIC_LIBRARY")
                list(APPEND TPDE_FLAGS "$<TARGET_FILE:${TPDE_LIB_TARGET}>")
            endif ()
        endif ()
    endforeach ()
    list(JOIN TPDE_FLAGS " " TPDE_FLAGS)
    target_compile_definitions(spicetest PRIVATE SPICE_TPDE_FLAGS="${TPDE_FLAGS}")
endif ()
# Enable pedantic warnings
target_compile_options(spicetest PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas ${SPICE_EXTRA_COMPILE_OPTIONS})
# Add test for IDEs
add_test(NAME spicetest COMMAND spicetest)

# Symlinks
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/bootstrap SYMBOLIC)

################# spicetest_leakcheck ################

add_custom_target(spicetest_leakcheck COMMAND valgrind --leak-check=full --suppressions=${CMAKE_SOURCE_DIR}/valgrind.supp --error-exitcode=1 ./spicetest DEPENDS spicetest)

################# spicetest_parallel #################

# Run the integration/unit test suite in parallel via gtest-parallel (one process per test case). The test runner writes its
# build artifacts to a per-test directory (see TestUtil::prepareArtifactDir), which makes the cases safe to run concurrently.
# Arguments forwarded to each spicetest process spawned by gtest-parallel (e.g. "--is-github-actions" in CI).
set(SPICE_TEST_ARGS "" CACHE STRING "Arguments forwarded to spicetest when run via the spicetest_parallel target")
# Collect all LLVM include dirs into a single "-I<dir> -I<dir> ..." string forwarded as one env var.
SET(LLVM_INCLUDE_FLAGS ${LLVM_INCLUDE_DIRS})
list(TRANSFORM LLVM_INCLUDE_FLAGS PREPEND "-I")
list(JOIN LLVM_INCLUDE_FLAGS " " LLVM_INCLUDE_FLAGS)
find_program(PYTHON3_EXECUTABLE NAMES python3 python)
if (PYTHON3_EXECUTABLE)
    add_custom_target(spicetest_parallel
            COMMAND ${CMAKE_COMMAND} -E env
            SPICE_STD_DIR=${CMAKE_SOURCE_DIR}/std
            SPICE_BOOTSTRAP_DIR=${CMAKE_SOURCE_DIR}/src
            LLVM_LIB_DIR=${LLVM_LIBRARY_DIRS}
            LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_FLAGS}
            ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/deps/gtest-parallel/gtest-parallel $<TARGET_FILE:spicetest> -- ${SPICE_TEST_ARGS}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS spicetest
            USES_TERMINAL
            COMMENT "Running spicetest in parallel via gtest-parallel")
endif ()

################# spicetest_bootstrap ################

# Build the bootstrap compiler with the host compiler, then run the reference test suites against it
add_custom_target(spicetest_bootstrap
        COMMAND ${CMAKE_COMMAND} -E env
        SPICE_STD_DIR=${CMAKE_SOURCE_DIR}/std
        SPICE_BOOTSTRAP_DIR=${CMAKE_SOURCE_DIR}/src
        LLVM_LIB_DIR=${LLVM_LIBRARY_DIRS}
        LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_FLAGS}
        $<TARGET_FILE:spicetest> --bootstrap ${SPICE_TEST_ARGS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        DEPENDS spicetest
        USES_TERMINAL
        COMMENT "Running spicetest against the bootstrap compiler")

############ spicetest_bootstrap_parallel #############

# Build the bootstrap compiler once, then run the reference test suites against it in parallel via gtest-parallel (one process
# per test case). Like for spicetest_parallel, every test case writes its build artifacts to its own directory.
set(SPICE_BOOTSTRAP_COMPILER_PATH ${CMAKE_CURRENT_BINARY_DIR}/test-tmp/bootstrap-compiler/spice${CMAKE_EXECUTABLE_SUFFIX})
if (PYTHON3_EXECUTABLE)
    add_custom_target(spicetest_bootstrap_parallel
            COMMAND ${CMAKE_COMMAND} -E env
            SPICE_STD_DIR=${CMAKE_SOURCE_DIR}/std
            SPICE_BOOTSTRAP_DIR=${CMAKE_SOURCE_DIR}/src
            LLVM_LIB_DIR=${LLVM_LIBRARY_DIRS}
            LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_FLAGS}
            $<TARGET_FILE:spicetest> --bootstrap-build-only ${SPICE_TEST_ARGS}
            COMMAND ${CMAKE_COMMAND} -E env
            SPICE_STD_DIR=${CMAKE_SOURCE_DIR}/std
            SPICE_BOOTSTRAP_DIR=${CMAKE_SOURCE_DIR}/src
            LLVM_LIB_DIR=${LLVM_LIBRARY_DIRS}
            LLVM_INCLUDE_DIRS=${LLVM_INCLUDE_FLAGS}
            ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/deps/gtest-parallel/gtest-parallel $<TARGET_FILE:spicetest> --
            --bootstrap-compiler=${SPICE_BOOTSTRAP_COMPILER_PATH} ${SPICE_TEST_ARGS}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS spicetest
            USES_TERMINAL
            COMMENT "Running spicetest against the bootstrap compiler in parallel via gtest-parallel")
endif ()
