cmake_minimum_required(VERSION 3.6)
project(data_transmission_shedule)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")


set(SOURCE_FILES src/main.cpp src/Algorithm.cpp src/Algorithm.h src/Job.cpp src/Job.h src/Parser.cpp src/Parser.h src/Exceptions.h src/Types.h src/JobWrapper.cpp src/JobWrapper.h)
add_executable(prog_02160506_521 ${SOURCE_FILES})