cmake_minimum_required(VERSION 3.20)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

find_program(SCCACHE_EXEC sccache)
find_program(CCACHE_EXEC ccache)
if(SCCACHE_EXEC)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${SCCACHE_EXEC})
elseif(CCACHE_EXEC)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE_EXEC})
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2 SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE inc ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <SDL2/SDL.h>
    <SDL2/SDL_ttf.h>
    <vector>
    <array>
    <cmath>
)