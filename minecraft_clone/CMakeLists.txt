cmake_minimum_required(VERSION 3.5.0)
project(minecraft_clone VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

if(CMAKE_SYSTEM MATCHES Windows)
    message(STATUS "Target System is Windows")
endif()

if(CMAKE_SYSTEM MATCHES Linux)
    message(STATUS "Target System is Linux")
endif()

if(CMAKE_HOST_SYSTEM MATCHES Linux)
    message(STATUS "Build Host System is Linux")
endif()

file(GLOB IMGUI_SOURCES
    "src/vendor/imgui/*.cpp"
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "src/vendor/imgui")

set(GLFW_ROOT ${CMAKE_SOURCE_DIR}/glfw-windows)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchain.cmake")
find_package(glfw3 REQUIRED PATHS ${GLFW_ROOT})

find_package(OpenGL REQUIRED)

add_executable(minecraft_clone 
    src/glad.h
    src/glad.c
    src/vendor/stb_image/stb_image.cpp
    src/texture.cpp
    src/commonInclude.cpp
    src/renderer.cpp
    src/shader.cpp
    src/indexBuffer.cpp
    src/vertexBuffer.cpp
    src/vertexArray.cpp
    src/main.cpp
)

target_include_directories(minecraft_clone PRIVATE ${GLFW_ROOT}/include)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(minecraft_clone
    m
    imgui
    glfw
    OpenGL::GL
)