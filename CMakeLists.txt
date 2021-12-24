cmake_minimum_required(VERSION 3.5)

project(imaqliq-task VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(test)

set(SERVER_SOURCES src/server.cpp)
set(CLIENT_SOURCES src/client.cpp)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

message("server sources  " ${SERVER_SOURCES})
message("client sources  " ${CLIENT_SOURCES})

add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})

target_include_directories(server PUBLIC src/)
target_include_directories(client PUBLIC src/)
target_link_libraries(server Threads::Threads)