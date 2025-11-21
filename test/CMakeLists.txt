# Set ANTLR grammar
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/../src/Spice.g4 VISITOR PACKAGE spice::compiler)

set(SOURCES
        main.cpp
        driver/TestDriver.cpp
        TestRunner.cpp
        util/TestUtil.cpp
        unittest/UnitBlockAllocator.cpp
        unittest/UnitCommonUtil.cpp
        unittest/UnitFileUtil.cpp
        unittest/UnitSystemUtil.cpp
        unittest/UnitDriver.cpp)

add_executable(spicetest ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Enable pedantic warnings
target_compile_options(spicetest PRIVATE -Wpedantic -Wall -Werror -Wno-unknown-pragmas)

# Include Antlr components
include_directories(
        ${ANTLR_Spice_OUTPUT_DIR}
        ../lib/antlr4/runtime/Cpp/runtime/src
)

add_test(NAME spicetest COMMAND spicetest)
target_link_libraries(spicetest PUBLIC spicecore gtest gmock antlr4_static ${LLVM_LIBS})

# Test files symlink
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)

# Bootstrap compiler symlink
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/../src-bootstrap ${CMAKE_CURRENT_BINARY_DIR}/bootstrap SYMBOLIC)

# Add leak check target
add_custom_target(spicetestleakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spicetest DEPENDS spicetest)