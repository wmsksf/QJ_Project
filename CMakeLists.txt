cmake_minimum_required(VERSION 3.15)
project(project_jj)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(project_jj
        DataTypes.cpp
        DataTypes.h
        main.cpp
        Utils.cpp
        Utils.h
        LinkedList.h
        LinkedList.cpp
        Stack.cpp)
