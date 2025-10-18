cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL LANGUAGES C)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -z noexecstack")

find_package(glfw3 3.4 REQUIRED) 

add_executable(LearnOpenGL 
    src/main.c
    src/glad.c
    src/stb_image.c
    src/camera.c
    src/shape_factory.c
    src/snake.c)

target_include_directories(LearnOpenGL 
    PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(LearnOpenGL glfw GL pthread Xrandr Xi dl m)
