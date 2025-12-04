cmake_minimum_required(VERSION 3.25)

project(triangle LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Vulkan REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW3 REQUIRED glfw3)

include_directories(
${Vulkan_INCLUDE_DIRS}
${GLFW3_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}
src/main.cpp
src/lve_window.cpp
src/lve_pipeline.cpp
src/first_app.cpp
src/lve_device.cpp
)

target_link_libraries(${PROJECT_NAME}
${Vulkan_LIBRARIES}
${GLFW3_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
