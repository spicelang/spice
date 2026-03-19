# Initialize GTest
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GTest REQUIRED)

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
target_compile_options(spicetest PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas)
# Add test for IDEs
add_test(NAME spicetest COMMAND spicetest)

# Symlinks
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)
file(CREATE_LINK ${CMAKE_SOURCE_DIR}/src-bootstrap ${CMAKE_CURRENT_BINARY_DIR}/bootstrap SYMBOLIC)

################# spicetest_leakcheck ################

add_custom_target(spicetest_leakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spicetest DEPENDS spicetest)
