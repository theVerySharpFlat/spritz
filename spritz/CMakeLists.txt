project(spritz_library)

set(SPRITZ_SOURCES
    src/spritz.c
    src/spritz_window.c
    src/spritz_graphics_api.c
    src/spritz_graphics_api_gl.c)

add_library(spritz ${SPRITZ_SOURCES})

target_include_directories(spritz PUBLIC include)
target_include_directories(spritz PRIVATE src)

add_subdirectory(third_party/glfw)
add_subdirectory(third_party/glad)


target_link_libraries(spritz PRIVATE glfw glad)
