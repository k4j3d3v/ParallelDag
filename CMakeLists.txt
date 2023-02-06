cmake_minimum_required(VERSION 3.23)
project(ParallelDag)

set(CMAKE_CXX_STANDARD 20)
#set("-DSEQ")
#add_definitions(-DSEQ)
#set(THREADS_PREFER_PTHREAD_FLAG ON)


include_directories(include)
file(GLOB SOURCES "src/main_simple.cpp")
#file(GLOB SOURCES2 src/*.cpp)
#message("souces: ${SOURCES}")
add_executable(pardag ${SOURCES})
add_executable(seqdag ${SOURCES})

target_compile_definitions(seqdag PRIVATE SEQ)
#target_compile_options(seqdag
#        PRIVATE
#        -O3)