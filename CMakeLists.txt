cmake_minimum_required(VERSION 3.18)

# replace with your locations of gcc an g++
set(CMAKE_C_COMPILER "/usr/bin/gcc-11")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-11")

project(OpenGL-theCherno)

# replace with your preferred version of c++
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp src/Renderer.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp src/VertexArray.cpp src/VertexBufferLayout.cpp )

target_link_libraries(${PROJECT_NAME} GL glfw ${PROJECT_SOURCE_DIR}/Dependencies/glew/lib/libGLEW.so.2.1.0)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Dependencies/glew/include)