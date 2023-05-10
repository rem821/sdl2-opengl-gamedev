cmake_minimum_required(VERSION 3.21)

set(PLATFORM_LINUX 1)
set(ENABLE_ASSERTS 1)

set(ENGINE_NAME VulkanEngine)
project(${ENGINE_NAME} VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
# Turn on all possible warnings during build
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -fPIC")

# Setup Debug and Release flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
add_compile_options(-Wno-unused-parameter -Wno-unused-variable)


# Compile Engine as a shared library
set(ENGINE_SOURCES
        vulkan-engine/src/logging/Log.cpp
        vulkan-engine/src/platform/linux/LinuxWindow.cpp
        vulkan-engine/src/Layer.cpp
        vulkan-engine/src/LayerStack.cpp
        vulkan-engine/src/Application.cpp
        vulkan-engine/src/imgui/ImGuiLayer.cpp
        vulkan-engine/src/platform/vulkan/VulkanDevice.cpp
        vulkan-engine/src/platform/vulkan/VulkanRenderer.cpp
        vulkan-engine/src/platform/vulkan/VulkanSwapChain.cpp
        vulkan-engine/src/platform/vulkan/VulkanDescriptors.cpp
        vulkan-engine/src/platform/vulkan/VulkanPipeline.cpp
        vulkan-engine/src/platform/vulkan/VulkanBuffer.cpp
        vulkan-engine/src/platform/vulkan/VulkanModel.cpp
        vulkan-engine/src/platform/vulkan/VulkanRenderSystem.cpp
        vulkan-engine/src/platform/linux/LinuxInput.cpp
        vulkan-engine/src/rendering/Camera.cpp
        vulkan-engine/src/imgui/ImGuiBuild.cpp
        vulkan-engine/src/platform/vulkan/VulkanContext.cpp)
#file(GLOB_RECURSE ENGINE_SOURCES vulkan-engine/src/*.cpp)
file(GLOB_RECURSE ENGINE_HEADERS vulkan-engine/include/*.h)

add_library(${ENGINE_NAME} STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS} vulkan-engine/libs/thread-pool/BS_thread_pool.hpp)
target_include_directories(${ENGINE_NAME} PUBLIC vulkan-engine/include)

### <-- Engine dependencies -->
add_subdirectory(vulkan-engine/libs/glfw)
add_subdirectory(vulkan-engine/libs/glm)
add_subdirectory(vulkan-engine/libs/tinyobjloader)
add_subdirectory(vulkan-engine/libs/sqlitecpp)
add_subdirectory(vulkan-engine/libs/spdlog)

target_include_directories(${ENGINE_NAME} PRIVATE vulkan-engine/libs/glad/include vulkan-engine/libs/glad/include/glad)
target_include_directories(${ENGINE_NAME} PRIVATE vulkan-engine/libs/glfw/include)
target_include_directories(${ENGINE_NAME} PRIVATE vulkan-engine/libs/thread-pool)
target_include_directories(${ENGINE_NAME} PRIVATE vulkan-engine/libs/imgui vulkan-engine/libs/imgui/backends)

find_package(Vulkan REQUIRED)
target_include_directories(${ENGINE_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})

# ImGui
add_library(imgui STATIC
        vulkan-engine/libs/imgui/imgui.cpp
        vulkan-engine/libs/imgui/imgui_draw.cpp
        vulkan-engine/libs/imgui/imgui_demo.cpp
        vulkan-engine/libs/imgui/imgui_widgets.cpp
        vulkan-engine/libs/imgui/imgui_tables.cpp
        vulkan-engine/libs/imgui/backends/imgui_impl_glfw.cpp
        vulkan-engine/libs/imgui/backends/imgui_impl_vulkan.cpp
        )
target_include_directories(imgui PRIVATE vulkan-engine/libs/imgui vulkan-engine/libs/glfw/include)
target_link_libraries(imgui PRIVATE glfw)

target_link_libraries(${ENGINE_NAME} ${Vulkan_LIBRARIES} glfw glm SQLiteCpp sqlite3 tinyobjloader imgui pthread fmt dl)
target_compile_definitions(${ENGINE_NAME} PRIVATE
        PLATFORM_LINUX=${PLATFORM_LINUX}
        ENABLE_ASSERTS=${ENABLE_ASSERTS}
        BUILD_DLL=1
        )
target_compile_options(${ENGINE_NAME} PRIVATE ${Vulkan_COMPILE_OPTIONS})
target_precompile_headers(${ENGINE_NAME} PRIVATE vulkan-engine/include/precompiled_headers/PCH.h)
### <-- Engine dependencies -->


# Compile Sandbox and link with vulkan-engine
set(SANDBOX_NAME VulkanSandbox)
file(GLOB_RECURSE SANDBOX_SOURCES sandbox/*.cpp)
add_executable(${SANDBOX_NAME} sandbox/SandboxApp.cpp)
target_sources(${SANDBOX_NAME} PRIVATE ${SANDBOX_SOURCES})
target_include_directories(${SANDBOX_NAME} PRIVATE vulkan-engine)
target_link_libraries(${SANDBOX_NAME} ${ENGINE_NAME})
target_compile_definitions(${SANDBOX_NAME} PRIVATE PLATFORM_LINUX=${PLATFORM_LINUX} ENABLE_ASSERTS=${ENABLE_ASSERTS} BUILD_DLL=0)
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

foreach (GLSL ${GLSL_SOURCE_FILES})
    message(STATUS "BUILDING SHADER")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    message(STATUS ${GLSL})
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach (GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
#file(COPY models DESTINATION ${CMAKE_BINARY_DIR})