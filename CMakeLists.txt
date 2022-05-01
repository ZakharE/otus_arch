cmake_minimum_required(VERSION 3.20)
project(otus)

set(CMAKE_CXX_STANDARD 17)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()
find_package(GTest REQUIRED)

add_executable(otus main.cpp src/hw1/solver.h src/hw1/solver.test.cpp src/hw3/exception_handler.h src/hw3/command_handlers.h src/hw3/handler.test.cpp src/hw4/command.h src/hw4/interfaces.h src/test_shared/structs.h src/hw4/tests/burn_fuel.test.cpp src/hw5/ioc.h src/hw5/ioc.test.cpp src/test_shared/mock.h src/test_shared/structs.cpp)
target_link_libraries(otus ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(otus)