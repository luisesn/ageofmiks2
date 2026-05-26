cmake_minimum_required(VERSION 3.16)
project(AgeOfMiks2 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2       REQUIRED)
find_package(SDL2_ttf   REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(ZLIB       REQUIRED)

add_executable(AgeOfMiks2 main.cpp)

target_link_libraries(AgeOfMiks2 PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
    ZLIB::ZLIB
)

# Automatically copy all SDL2 and dependency DLLs to build output directory
add_custom_command(TARGET AgeOfMiks2 POST_BUILD
    # Copy SDL2 libraries
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL2::SDL2>"
        "$<TARGET_FILE_DIR:AgeOfMiks2>/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL2_ttf::SDL2_ttf>"
        "$<TARGET_FILE_DIR:AgeOfMiks2>/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:SDL2_mixer::SDL2_mixer>"
        "$<TARGET_FILE_DIR:AgeOfMiks2>/"
    # Then run the CMake script for remaining dependencies
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/copy_dlls.cmake
)
