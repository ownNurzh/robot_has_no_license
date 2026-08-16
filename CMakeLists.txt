cmake_minimum_required(VERSION 3.25)

project(robot_has_no_license LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(entryFile src/main.cpp)

#base include
target_include_directories(entryFile PRIVATE include)

#external

##raylib
target_include_directories(entryFile PRIVATE external/raylib/include)
target_link_libraries(entryFile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib/lib/raylibdll.lib")

##copy raylib dll
add_custom_command(TARGET entryFile POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/external/raylib/lib/raylib.dll"
        $<TARGET_FILE_DIR:entryFile>
    COMMAND_EXPAND_LISTS
)