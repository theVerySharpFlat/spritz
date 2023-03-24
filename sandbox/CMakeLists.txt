project(spritz_sandbox)

add_executable(spritz_sandbox sandbox.c stbimage.c)

target_link_libraries(spritz_sandbox PUBLIC spritz)
