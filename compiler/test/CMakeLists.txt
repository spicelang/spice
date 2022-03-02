# Source files to compile
set(SOURCES
        main.cpp
        AnalyzerTest.cpp
        GeneratorTest.cpp
        StdTest.cpp
        TestUtil.cpp
        TestUtil.h)

# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/../src/grammar/Spice.g4 VISITOR)

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

# Add executable target: Spice_test
add_executable(${CMAKE_PROJECT_NAME}_test ${ANTLR_Spice_CXX_OUTPUTS} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC antlr4_static ${LLVM_LIBS} ${CMAKE_PROJECT_NAME}_lib gtest)

# Tell CMake to run the tests in the Spice_test executable
enable_testing()
add_test(NAME test COMMAND ${CMAKE_PROJECT_NAME}_test)

# Link test files
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)