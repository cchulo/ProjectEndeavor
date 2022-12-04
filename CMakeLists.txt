cmake_minimum_required(VERSION 3.23)
project(ProjectEndeavor)

set(CMAKE_CXX_STANDARD 20)

message("building with ${CMAKE_BUILD_TYPE}")

# Conan setup
message("discovering conan binaries")
include(${PROJECT_SOURCE_DIR}/bin/conan/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(HelloTriangleApp app/HelloTriangleApp/main.cpp)
add_executable(CosmosVk app/CosmosVk/main.cpp)

message("compiling shaders to ${CMAKE_BINARY_DIR}/bin/shaders")

find_package(Vulkan REQUIRED)
target_link_libraries(CosmosVk ${CONAN_LIBS} Vulkan::Vulkan)
target_link_libraries(HelloTriangleApp ${CONAN_LIBS} Vulkan::Vulkan)
