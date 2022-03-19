# Find ANTLR
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/../src/grammar/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        AnalyzerTest.cpp
        GeneratorTest.cpp
        StdTest.cpp
        TestUtil.cpp
        TestUtil.h)

add_executable(${CMAKE_PROJECT_NAME}_test ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

add_test(NAME ${CMAKE_PROJECT_NAME}_test COMMAND ${CMAKE_PROJECT_NAME}_test)
target_link_libraries(${CMAKE_PROJECT_NAME}_test PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest antlr4_static ${LLVM_LIBS})

# Test files
file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR}/test-files ${CMAKE_CURRENT_BINARY_DIR}/test-files SYMBOLIC)