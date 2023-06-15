cmake_minimum_required(VERSION 3.25)
project(Synchronization_task)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(Synchronization_task
        Storage.cpp
        Storage.h
        SynchronizationTask.cpp Buffer.cpp Buffer.h)
