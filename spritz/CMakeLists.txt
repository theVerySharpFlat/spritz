project(spritz_library)

set(SPRITZ_SOURCES
    src/spritz.c
    src/window.c
    src/graphics_api.c
    src/graphics_api_gl.c
    src/graphics_api_gl_shaders.c
    src/renderer.c
    src/camera.c
    src/time.c)

add_library(spritz ${SPRITZ_SOURCES})

target_include_directories(spritz PUBLIC include)
target_include_directories(spritz PRIVATE src)

add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)
add_subdirectory(third_party/cglm)

find_package(OpenGL)

if(SPRITZ_ENABLE_VULKAN)
    # Install the Vulkan SDK from LunarG!!!
    find_package(Vulkan REQUIRED)

    add_subdirectory(third_party/volk)

    target_link_libraries(spritz PRIVATE volk)

    target_sources(spritz PRIVATE src/graphics_api_vk.c)
endif()

target_link_libraries(spritz PRIVATE OpenGL::GL glfw glad cglm)
