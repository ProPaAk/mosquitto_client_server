cmake_minimum_required(VERSION 3.2)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

add_subdirectory(cpp_client)
add_subdirectory(cpp_server)
