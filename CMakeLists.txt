cmake_minimum_required(VERSION 3.10)
project(AntNet)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")

# SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(AntNet
        LICENSE
        README.md
        src/main.cpp
        src/ObjectFactory.cpp
        src/Component.cpp
        src/GameObject.cpp
        src/System.cpp
        src/Engine.cpp
        src/HandleManager.cpp)

target_include_directories(AntNet PRIVATE
        ${CMAKE_SOURCE_DIR}/include/
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})

target_link_libraries(AntNet
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES})

