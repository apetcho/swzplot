cmake_minimum_required(VERSION 3.17)
project(swzplot LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

# FetchContent_Declare(
#     gl2ps
#     GIT_REPOSITORY https://github.com/Open-Cascade-SAS/gl2ps.git
# )
# FetchContent_MakeAvailable(gl2ps)

# FetchContent_Declare(
#     freeglut
#     GIT_REPOSITORY https://github.com/apetcho/freeglut.git
# )
# FetchContent_MakeAvailable(freeglut)

# -*-
add_subdirectory(src)