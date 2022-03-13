cmake_minimum_required(VERSION 3.20)
project(otus)

set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()
find_package(GTest REQUIRED)

add_executable(otus main.cpp func.h test/some.cpp)
target_link_libraries(otus ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(otus)