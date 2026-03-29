cmake_minimum_required(VERSION 4.1)
project(EukariotSDL2)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenGL REQUIRED )
find_package( GLU REQUIRED )

add_executable(EukariotSDL2 main.cpp LUtil.cpp headers/LOpenGL.h headers/LUtil.h
        headers/shader.h headers/stb_image.h)
add_subdirectory(vendor/SDL2)
add_subdirectory(vendor/SDL_image)
add_subdirectory(vendor/GLEW)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image libglew_shared OpenGL::GLU)

if (WIN32)
    target_link_libraries(
            Sandbox PRIVATE SDL2main
    )
endif()

