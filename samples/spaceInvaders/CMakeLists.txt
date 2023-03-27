project(SpaceInvaders)

add_executable(spaceInvaders spaceInvaders.cpp Invader.cpp InvaderManager.cpp Ship.cpp Bullet.cpp stbimage.c)

target_link_libraries(spaceInvaders PUBLIC spritz)
target_compile_features(spaceInvaders PUBLIC cxx_std_17)
