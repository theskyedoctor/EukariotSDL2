cmake_minimum_required(VERSION 4.1)
project(EukariotSDL2)

set(CMAKE_CXX_STANDARD 20)

add_executable(EukariotSDL2 main.cpp)
add_subdirectory(vendor/SDL2)

target_include_directories(${PROJECT_NAME}
        PUBLIC libs/SDL2/include
)

target_link_directories(${PROJECT_NAME}
        PUBLIC libs/SDL2/src
)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static)


