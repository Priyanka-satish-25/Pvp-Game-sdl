cmake_minimum_required(VERSION 3.16)
project(pvp)

set(CMAKE_CXX_STANDARD 11)

# Find SDL2, SDL2_image, and SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Include SDL2, SDL2_image, SDL2_ttf headers
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add the source files to the executable
add_executable(pvp main.cpp Window_Object.cpp Map_Object.cpp Player_Object.cpp Pellets_Object.cpp Player2_Object.cpp)

# Link SDL2, SDL2_image, SDL2_ttf libraries
target_link_libraries(pvp SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

# Optionally, add the path to your library files if needed
link_directories(/opt/homebrew/lib)
