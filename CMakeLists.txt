cmake_minimum_required(VERSION 3.0)
project(libmoor_all C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libmoor)

add_executable(moortest moortest.c)
target_link_libraries(moortest moor)

add_executable(moorextract moorextract.c)
target_link_libraries(moorextract moor)

add_executable(moorconvert moorconvert.c)
target_link_libraries(moorconvert moor)