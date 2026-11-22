cmake_minimum_required(VERSION 3.24)

project("Rift")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REFLECTION_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/Intermative")
set(REFLECTION_TOOL         "${CMAKE_SOURCE_DIR}/Tools/HeaderTool.py")
set(ENGINE_ICON_PATH        "${CMAKE_SOURCE_DIR}/app.ico")

find_package(Python3 REQUIRED COMPONENTS Interpreter)
if(NOT Python3_EXECUTABLE)
    message(FATAL_ERROR "Python3 interpreter not found, needed for some Tools")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(Output)
include(Reflection)

if (WIN32)
    include(Windows)
endif()

add_subdirectory("Source/Core")
add_subdirectory("Source/Math")
#add_subdirectory("Source/Engine")
add_subdirectory("Source/Runtime")
add_subdirectory("Source/RHI")
#add_subdirectory("Source/RiftEditor")