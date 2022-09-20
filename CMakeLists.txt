cmake_minimum_required(VERSION 3.23)
project(ProjectEndeavor)

set(CMAKE_CXX_STANDARD 20)

# Conan setup
include(${PROJECT_SOURCE_DIR}/bin/conan/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(HelloTriangleApp app/hello-triangle-application.cpp)
add_executable(ProjectEndeavor app/main.cpp)
find_package(Vulkan REQUIRED)
target_link_libraries(ProjectEndeavor ${CONAN_LIBS} Vulkan::Vulkan)
target_link_libraries(HelloTriangleApp ${CONAN_LIBS} Vulkan::Vulkan)
