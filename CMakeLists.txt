cmake_minimum_required(VERSION 3.16)
project(ParallelDag)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)
file(GLOB SOURCES "src/main_worstscen.cpp")

add_executable(pardag ${SOURCES})
add_executable(seqdag ${SOURCES})
target_link_libraries(pardag PRIVATE pthread)
target_compile_definitions(seqdag PRIVATE SEQ)
target_compile_options(seqdag PRIVATE -O3)