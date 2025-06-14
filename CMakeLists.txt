cmake_minimum_required(VERSION 3.6)
project(turboengine LANGUAGES CXX)

include_directories(turboengine include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
include(FindALSA)

FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.16
    GIT_SHALLOW ON
    SYSTEM)
FetchContent_MakeAvailable(SDL3)

# set(CMAKE_TARGET)

add_subdirectory(src)