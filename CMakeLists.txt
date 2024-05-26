cmake_minimum_required(VERSION 3.26)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

# fetching libraries from git
include(FetchContent)

FetchContent_Declare(
  mosquitto
  GIT_REPOSITORY https://github.com/eclipse/mosquitto.git
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/mosquitto
  BUILD_COMMAND "WITH_LIB_CPP=ON"
)
FetchContent_MakeAvailable(mosquitto)

set(HelpersDir ${CMAKE_SOURCE_DIR}/helpers)

add_subdirectory(c_style_version)
add_subdirectory(cpp_style_version)
