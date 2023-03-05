project(spritz_library)

set(SPRITZ_SOURCES
    src/spritz.c
    src/window.c
    src/graphics_api.c
    src/graphics_api_gl.c
    src/graphics_api_gl_shaders.c
    src/renderer.c)

add_library(spritz ${SPRITZ_SOURCES})

target_include_directories(spritz PUBLIC include)
target_include_directories(spritz PRIVATE src)

add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)

find_package(OpenGL)
target_link_libraries(spritz PRIVATE OpenGL::GL glfw glad)
