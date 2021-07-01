set(BINARY ${CMAKE_PROJECT_NAME})

set(CMAKE_CXX_COMPILER_ID GNU)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(ANTLR4CPP_JAR_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/antlr/antlr-4.9.2-complete.jar)
include(ExternalAntlr4Cpp)
include_directories(${ANTLR4CPP_INCLUDE_DIRS})
link_directories(${ANTLR4CPP_LIBS})
message(STATUS "Found antlr4cpp libs: ${ANTLR4CPP_LIBS} and includes: ${ANTLR4CPP_INCLUDE_DIRS} ")
antlr4cpp_process_grammar(Spice spice
        ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4
        ${CMAKE_CURRENT_SOURCE_DIR}/grammar/Spice.g4)
include_directories(${antlr4cpp_include_dirs_spice})

set(SOURCES
        main.cpp
        ${antlr4cpp_src_files_spice})

add_executable(${BINARY}_run ${SOURCES})

add_dependencies(${BINARY}_run antlr4cpp antlr4cpp_generation_spice)
target_link_libraries(${BINARY}_run antlr4-runtime)

add_library(${BINARY}_lib STATIC ${SOURCES})