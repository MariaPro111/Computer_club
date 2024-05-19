cmake_minimum_required(VERSION 3.28)
project(my_club)

set(CMAKE_CXX_STANDARD 17)

add_executable(my_club main.cpp
        table.cpp
        club.cpp
        event.cpp
        time.cpp)
