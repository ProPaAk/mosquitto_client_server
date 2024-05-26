cmake_minimum_required(VERSION 3.26)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

add_subdirectory(c_client)
add_subdirectory(c_server)
