cmake_minimum_required(VERSION 3.5)
project(imaqliq-task-test VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(GTEST_ROOT /home/pizhun/c_cpp_tst/googletest/)
set(GTEST_INCLUDE_DIR /home/pizhun/c_cpp_tst/googletest/googletest/include/ /home/pizhun/c_cpp_tst/googletest/googlemock/include/)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

enable_testing()
find_package(GTest REQUIRED)

add_executable(test-server test-server.cpp ../src/server.cpp)
add_executable(test-client test-client.cpp ../src/client.cpp)

target_include_directories(test-server PUBLIC ../src/ ${GTEST_INCLUDE_DIR})
target_include_directories(test-client PUBLIC ../src/)

target_compile_definitions(test-server PUBLIC UNIT_TEST)
target_compile_definitions(test-client PUBLIC UNIT_TEST)
target_link_libraries(test-server ${GTEST_BOTH_LIBRARIES} Threads::Threads)
target_link_libraries(test-client ${GTEST_BOTH_LIBRARIES} Threads::Threads)