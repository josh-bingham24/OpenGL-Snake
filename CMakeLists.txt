cmake_minimum_required(VERSION 3.10)
project(OpenGL-Snake LANGUAGES C)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")

find_package(glfw3 3.4 REQUIRED) 

add_executable(OpenGL-Snake 
    src/main.c
    src/glad.c
    src/stb_image.c
    src/camera.c
    src/shape_factory.c
    src/snake.c)

target_include_directories(OpenGL-Snake 
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(OpenGL-Snake glfw GL pthread Xrandr Xi dl m)
