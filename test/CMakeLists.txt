# Set ANTLR grammar
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/../src/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        driver/Driver.cpp
        driver/Driver.h
        TestRunner.cpp
        util/TestUtil.cpp
        util/TestUtil.h
        unittest/UnitBlockAllocator.cpp
        unittest/UnitCommonUtil.cpp
        unittest/UnitDriver.cpp)

add_executable(spicetest ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Enable pedantic warnings
target_compile_options(spicetest PRIVATE -Wpedantic -Wall)

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

add_test(NAME spicetest COMMAND spicetest)
target_link_libraries(spicetest PUBLIC spicecore gtest gmock antlr4_static ${LLVM_LIBS})

# Test files symlink
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)

# Bootstrap compiler symlink
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/../src-bootstrap ${CMAKE_CURRENT_BINARY_DIR}/bootstrap SYMBOLIC)

# Add leak check target
add_custom_target(spicetestleakcheck COMMAND valgrind --leak-check=full --error-exitcode=1 ./spicetest DEPENDS spicetest)