# Initialize GTest (git submodule in deps/)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/googletest ${CMAKE_BINARY_DIR}/deps/googletest EXCLUDE_FROM_ALL)

################ spicetest executable ################

set(SOURCES
        # Test fixture
        main.cpp
        TestRunner.cpp
        driver/TestDriver.cpp
        util/TestUtil.cpp
        # Unit tests
        unittest/UnitBlockAllocator.cpp
        unittest/UnitCommonUtil.cpp
        unittest/UnitFileUtil.cpp
        unittest/UnitSystemUtil.cpp
        unittest/UnitDriver.cpp
)

add_executable(spicetest ${SOURCES})
# Link with spicecore, gtest, gmock and llvm
target_link_libraries(spicetest PRIVATE spicecore GTest::gtest GTest::gmock ${LLVM_LIBS})
# Enable pedantic warnings
target_compile_options(spicetest PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas ${SPICE_EXTRA_COMPILE_OPTIONS})
# Add test for IDEs
add_test(NAME spicetest COMMAND spicetest)

# Symlinks
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/src-bootstrap ${CMAKE_CURRENT_BINARY_DIR}/bootstrap SYMBOLIC)

################# spicetest_leakcheck ################

add_custom_target(spicetest_leakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spicetest DEPENDS spicetest)

################# spicetest_parallel #################

# Run the integration/unit test suite in parallel via gtest-parallel (one process per test case). The test runner writes its
# build artifacts to a per-test directory (see TestUtil::prepareArtifactDir), which makes the cases safe to run concurrently.
# Arguments forwarded to each spicetest process spawned by gtest-parallel (e.g. "--is-github-actions" in CI).
set(SPICE_TEST_ARGS "" CACHE STRING "Arguments forwarded to spicetest when run via the spicetest_parallel target")
find_program(PYTHON3_EXECUTABLE NAMES python3 python)
if (PYTHON3_EXECUTABLE)
    # The test runner needs a few environment variables at runtime: the compiler reads SPICE_STD_DIR / SPICE_BOOTSTRAP_DIR via
    # SystemUtil, and the bootstrap-compiler tests shell-expand LLVM_LIB_DIR in their linker command. add_custom_target has no
    # ENVIRONMENT property, so the env is injected into the command itself via "cmake -E env" to make sure it reaches the
    # spicetest child processes spawned by gtest-parallel. Arguments after "--" are forwarded to each spicetest process.
    add_custom_target(spicetest_parallel
            COMMAND ${CMAKE_COMMAND} -E env
            SPICE_STD_DIR=${CMAKE_SOURCE_DIR}/std
            SPICE_BOOTSTRAP_DIR=${CMAKE_SOURCE_DIR}/src-bootstrap
            LLVM_LIB_DIR=${LLVM_LIBRARY_DIRS}
            ${PYTHON3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/deps/gtest-parallel/gtest-parallel $<TARGET_FILE:spicetest> -- ${SPICE_TEST_ARGS}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            DEPENDS spicetest
            USES_TERMINAL
            COMMENT "Running spicetest in parallel via gtest-parallel")
endif ()
