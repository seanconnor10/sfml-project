cmake_minimum_required(VERSION 3.31)
project(sfml_project)

set(CMAKE_CXX_STANDARD 17)

set(SFML_DIR ../SFML-3.0.0/lib/cmake/SFML)

add_executable(sfml_project src/main.cpp
        src/Camera.cpp
        src/Camera.h
        src/Player.cpp
        src/Player.h)

target_include_directories(sfml_project PUBLIC ../SFML-3.0.0/include)
target_link_directories(sfml_project PUBLIC ../SFML-3.0.0/lib) #bin not necessary?

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)

target_link_libraries(sfml_project PRIVATE opengl32 gdi32 glu32 SFML::Graphics SFML::Audio SFML::Network ) #opengl32 gdi32 glu32 not necessary?
