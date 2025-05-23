#cmake_minimum_required(VERSION 3.31)
#project(sfml_project)
#
#set(CMAKE_CXX_STANDARD 17)
#
#set(SFML_DIR ../SFML-3.0.0/lib/cmake/SFML)
#
#add_executable(sfml_project src/main.cpp
#        src/Camera.cpp
#        src/Camera.h
#        src/Player.cpp
#        src/Player.h
#        src/gl_light.h)
#
#target_include_directories(sfml_project PUBLIC ../SFML-3.0.0/include)
#target_link_directories(sfml_project PUBLIC ../SFML-3.0.0/lib ../SFML-3.0.0/bin) #bin not necessary?
#
#find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)
#
#target_link_libraries(sfml_project PRIVATE opengl32 gdi32 glu32 SFML::Graphics SFML::Audio SFML::Network ) #opengl32 gdi32 glu32 not necessary?

cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(sfml_project src/main.cpp
        src/Camera.cpp
        src/Camera.h
        src/Player.cpp
        src/Player.h
        src/gl_light.h)

target_compile_features(sfml_project PRIVATE cxx_std_17)
target_link_libraries(sfml_project PRIVATE SFML::Graphics)