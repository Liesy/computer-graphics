cmake_minimum_required(VERSION 3.17)
project(CLion)

include_directories(include)
link_directories(lib/x64)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

add_executable(CLion main5.cpp)

#target_link_libraries(CLion freeglut opengl32)
target_link_libraries(CLion libopengl32.a  libfreeglut.a  glu32.dll)