project(spritz_library)

set(SPRITZ_SOURCES
    src/spritz.c)

add_library(spritz ${SPRITZ_SOURCES})

target_include_directories(spritz PUBLIC include)
target_include_directories(spritz PRIVATE src)
