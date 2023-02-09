project(spritz_library)

set(SPRITZ_SOURCES
    src/spritz.c
    src/spritz_window.c)

add_library(spritz ${SPRITZ_SOURCES})

target_include_directories(spritz PUBLIC include)
target_include_directories(spritz PRIVATE src)

add_subdirectory(third_party/glfw)


target_link_libraries(spritz PRIVATE glfw)
