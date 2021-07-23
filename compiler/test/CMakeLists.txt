set(BINARY ${CMAKE_PROJECT_NAME}_test)

# Antlr
find_package(ANTLR REQUIRED)
antlr_target(Spice ${CMAKE_CURRENT_SOURCE_DIR}/../src/grammar/Spice.g4 VISITOR)

set(SOURCES
        main.cpp
        AnalyzerTest.cpp)

add_executable(${BINARY} ${SOURCES} ${ANTLR_Spice_CXX_OUTPUTS})

# Include Antlr components
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
include_directories(${ANTLR_Spice_OUTPUT_DIR})
include_directories(../lib/antlr4/runtime/Cpp/runtime/src)

add_test(NAME ${BINARY} COMMAND ${BINARY})
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest antlr4_static ${LLVM_LIBS})

# Test files
file(COPY test-files DESTINATION ${CMAKE_CURRENT_BINARY_DIR})