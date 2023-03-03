cmake_minimum_required(VERSION 3.21)

set(ENGINE_NAME VulkanEngine)

project(${ENGINE_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
# Turn on all possible warnings during build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fPIC")

# Setup Debug and Release flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# Compile Engine as a shared library
file(GLOB_RECURSE ENGINE_SOURCES vulkan-engine/*.cpp)
add_library(${ENGINE_NAME} SHARED)
target_include_directories(${ENGINE_NAME} PRIVATE libs vulkan-engine/include/logging)
target_precompile_headers(${ENGINE_NAME} PRIVATE vulkan-engine/include/precompiled_headers/PCH.h)

add_subdirectory(libs/sdl)
add_subdirectory(libs/glm)
add_subdirectory(libs/imgui)
add_subdirectory(libs/tinyobjloader)
add_subdirectory(libs/sqlite)
add_subdirectory(libs/spdlog)

find_package(Vulkan REQUIRED)

target_sources(${ENGINE_NAME} PRIVATE ${ENGINE_SOURCES})
target_link_libraries(${ENGINE_NAME} Vulkan::Vulkan SDL2 glm ImGui SQLiteCpp sqlite3 pthread fmt dl)
target_compile_definitions(${ENGINE_NAME} PRIVATE PLATFORM_LINUX=1 BUILD_DLL=1)

# Compile Sandbox and link with vulkan-engine
set(SANDBOX_NAME VulkanSandbox)
file(GLOB_RECURSE SANDBOX_SOURCES sandbox/*.cpp)
add_executable(${SANDBOX_NAME} sandbox/SandboxApp.cpp)
target_sources(${SANDBOX_NAME} PRIVATE ${SANDBOX_SOURCES})
target_include_directories(${SANDBOX_NAME} PRIVATE vulkan-engine)
target_link_libraries(${SANDBOX_NAME} ${ENGINE_NAME})
target_compile_definitions(${SANDBOX_NAME} PRIVATE PLATFORM_LINUX=1 BUILD_DLL=0)
############## Build SHADERS #######################

# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS
        ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}
        /usr/bin
        /usr/local/bin
        ${VULKAN_SDK_PATH}/Bin
        ${VULKAN_SDK_PATH}/Bin32
        $ENV{VULKAN_SDK}/Bin/
        $ENV{VULKAN_SDK}/Bin32/
        )

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
        )

foreach(GLSL ${GLSL_SOURCE_FILES})
    message(STATUS "BUILDING SHADER")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    message(STATUS ${GLSL})
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
#file(COPY models DESTINATION ${CMAKE_BINARY_DIR})