cmake_minimum_required(VERSION 3.18)
project(alq-cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
        alq
        Event.h
        Event.cpp
        Rung.h
        Rung.cpp
        ALadderQueue.h
        ALadderQueue.cpp
        AlqList.cpp
        AlqList.h
)


add_executable(main main.cpp)
target_link_libraries(main PRIVATE alq)

add_executable(test1 test1.cpp)
target_link_libraries(test1 PRIVATE alq)
