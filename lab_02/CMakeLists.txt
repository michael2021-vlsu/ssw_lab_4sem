cmake_minimum_required(VERSION 3.13)
project(lab_02)


set(CMAKE_CXX_STANDARD 17)
set(SRC ./src)

add_executable(lab_02 ${SRC}/*.cpp)
