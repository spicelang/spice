cmake_minimum_required(VERSION 3.17)
project(Spice)

set(CMAKE_CXX_STANDARD 20)

# Antlr
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/cmake)
add_definitions(-DANTLR4CPP_STATIC)
include(ExternalAntlr4Cpp)
set(ANTLR_EXECUTABLE ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/antlr/antlr-4.9.2-complete.jar)

# Main module
include_directories(src)
add_subdirectory(src)

# Test module
enable_testing()
add_subdirectory(test)
add_subdirectory(lib/googletest)

#IF(${WITH_TESTS})
#
#endif()