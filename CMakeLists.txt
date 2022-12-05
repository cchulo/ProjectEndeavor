cmake_minimum_required(VERSION 3.23)

# General build settings
project(ProjectEndeavor)
set(CMAKE_CXX_STANDARD 20)

# Echo system/build information
message("system name: ${CMAKE_SYSTEM_NAME}")
message("source directory: ${CMAKE_SOURCE_DIR}")
message("binary directory: ${CMAKE_BINARY_DIR}")
message("build type: ${CMAKE_BUILD_TYPE}")
message("architecture: ${CMAKE_SYSTEM_PROCESSOR}")

# Set output directories for anyone not using CLion
set(TARGET_DIR_FOR_CMAKE_FILES cmake-build-${CMAKE_BUILD_TYPE})
string(TOLOWER ${TARGET_DIR_FOR_CMAKE_FILES} TARGET_DIR_FOR_CMAKE_FILES)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin/${TARGET_DIR_FOR_CMAKE_FILES})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

message("cmake binary directory: ${CMAKE_BINARY_DIR}")
message("runtime output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Conan setup
message("discovering conan binaries")
include(${PROJECT_SOURCE_DIR}/bin/conan/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS) # We do not want conan to modify output directories

# Find locally installed packages
find_package(Vulkan REQUIRED)

# List of targets to generate build files
set(ENDEV_TARGETS HelloTriangleApp CosmosVk)

foreach (X IN LISTS ENDEV_TARGETS)
    message("generating ${X}")
    add_executable(${X} app/${X}/main.cpp)
    target_link_libraries(${X} ${CONAN_LIBS} Vulkan::Vulkan)

    set(X_BUILD_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}/${X}")

    # Each executable will be in its own folder
    set_target_properties(
            ${X}
            PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${X_BUILD_DIRECTORY}/lib"
            LIBRARY_OUTPUT_DIRECTORY "${X_BUILD_DIRECTORY}/lib"
            RUNTIME_OUTPUT_DIRECTORY "${X_BUILD_DIRECTORY}"
    )

    # Create shader folder
    set(SHADER_OUTPUT_DIR ${X_BUILD_DIRECTORY}/shaders)
    message("shaders will be compiled and output to ${SHADER_OUTPUT_DIR}")
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

    add_custom_command(
            TARGET ${X}
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Compiling shaders..."
    )

    file(GLOB CORE_SHADERS ${PROJECT_SOURCE_DIR}/src/core/shaders/*)
    foreach (SHADER_PATH IN LISTS CORE_SHADERS)
        get_filename_component(SHADER ${SHADER_PATH} NAME)
        add_custom_command(
                TARGET ${X}
                COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Compiling ${SHADER} to ${SHADER}.spv"
        )

        add_custom_command(
                TARGET ${X}
                COMMAND glslc ${SHADER_PATH} -o ${SHADER_OUTPUT_DIR}/${SHADER}.spv
                BYPRODUCTS ${SHADER_OUTPUT_DIR}/${SHADER}.spv
        )
    endforeach ()
endforeach ()
