cmake_minimum_required(VERSION 3.2)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -Wall -lrt")
set(CMAKE_C_LINKER_FLAGS “-g -Wall -Werror”)

add_executable(${ProjectId}
	main.cpp
	${HelpersDir}/helper_funcs.h
)

include_directories(${HelpersDir})
include_directories(${mosquitto_SOURCE_DIR})
target_link_libraries(${ProjectId} PUBLIC libmosquitto)
