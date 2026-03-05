cmake_minimum_required(VERSION 4.1)
project(EukariotSDL2)

set(CMAKE_CXX_STANDARD 20)

add_executable(EukariotSDL2 main.cpp)
add_subdirectory(vendor/SDL2)
add_subdirectory(vendor/SDL_image)
add_subdirectory(vendor/glew)

target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image glew OpenGL::GL)
if (WIN32)
    target_link_libraries(
            Sandbox PRIVATE SDL2main
    )
endif()

