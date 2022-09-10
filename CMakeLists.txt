cmake_minimum_required(VERSION 3.23)
project(ParallelDag)

set(CMAKE_CXX_STANDARD 20)
#set("-D SEQ")
#set(THREADS_PREFER_PTHREAD_FLAG ON)


include_directories(include)
file(GLOB SOURCES "*.cpp")

add_executable(pardag ${SOURCES})
