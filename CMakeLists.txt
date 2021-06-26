cmake_minimum_required(VERSION 3.17)
project(Spice)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_subdirectory(src)
IF(${WITH_TESTS})
    enable_testing()
    add_subdirectory(test)
    add_subdirectory(lib/googletest)
endif()