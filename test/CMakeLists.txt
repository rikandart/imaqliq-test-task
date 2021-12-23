cmake_minimum_required(VERSION 3.5)
project(imaqliq-task-test VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(GTEST_ROOT /home/pizhun/c_cpp_tst/googletest/)
set(GTEST_INCLUDE_DIR /home/pizhun/c_cpp_tst/googletest/googletest/include/ /home/pizhun/c_cpp_tst/googletest/googlemock/include/)

enable_testing()
find_package(GTest REQUIRED)

add_compile_definitions(UNIT_TEST)
add_executable(test-server test-server.cpp ../src/server.cpp)
# add_executable(test-client test-client.cpp ../src/client.cpp)

target_include_directories(test-server PUBLIC ../src/ ${GTEST_INCLUDE_DIR})
# target_include_directories(test-client PUBLIC ../src/)

target_link_libraries(test-server ${GTEST_BOTH_LIBRARIES})
# target_link_libraries(test-client ${GTEST_BOTH_LIBRARIES})