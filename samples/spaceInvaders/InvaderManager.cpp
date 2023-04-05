#include "InvaderManager.h"
#include "CollisionUtil.h"
#include "Invader.h"
#include "WindowConstants.h"
#include <cstdlib>
#include <algorithm>

void InvaderManager::spawnLargeInvader(SpritzRendererTextureHandle_t texture,
                                       float x, float y) {
    invaders.emplace_back(texture, x, y, InvaderConstants::largeInvaderWidth);
}

void InvaderManager::spawnNLargeInvaders(
    SpritzRendererTextureHandle_t textureHandle, int n) {
    float spaceBetween = WindowConstants::width / (float)(n + 1);

    for (int i = 0; i < n; i++) {
        spawnLargeInvader(textureHandle,
                          -WindowConstants::width / 2.0f + (i + 1) * spaceBetween,
                          WindowConstants::height / 2.0f +
                              InvaderConstants::largeInvaderWidth / 2.0f);
    }
}

void InvaderManager::update(double deltaTime, SpritzWindow_t window,
                            Ship& ship) {
    // perform collision detections
    uint32_t preNInvaders = invaders.size();

    bool canSpawnLargeInvader = true;
    CollisionUtil::Rect spawnRect = {
        .x = 0.0f,
        .y = InvaderConstants::largeInvaderWidth / 2.0f +
             WindowConstants::height / 2.0f,
        .width = WindowConstants::width,
        .height = InvaderConstants::largeInvaderWidth};
    for (int i = 0; i < preNInvaders; i++) {

        if (CollisionUtil::rectCollidesWithRect(
                spawnRect, invaders[i].getCollisionRect()) ||
            CollisionUtil::rectCollidesWithRect(invaders[i].getCollisionRect(),
                                                spawnRect)) {
            canSpawnLargeInvader = false;
            break;
        }
    }

    if (canSpawnLargeInvader) {
        spawnNLargeInvaders(ship.textureMap, (int)(std::rand() % 2) + 1);
    }

    preNInvaders = invaders.size();

    for (int i = 0; i < preNInvaders; i++) {
        if(invaders[i].collidesWith(ship)) {
            std::cout << "you lost! :(" << std::endl;
            std::exit(0);
        }
        for (int j = 0; j < ship.getBullets().size(); j++) {
            if (!ship.getBullets()[j].shouldDelete() &&
                invaders[i].collidesWith(ship.getBullets()[j])) {
                ship.getBullets()[j].setShouldDelete(true);
                invaders[i].setShouldDelete(true);

                if (invaders[i].getWidth() ==
                        InvaderConstants::largeInvaderWidth ||
                    invaders[i].getWidth() ==
                        InvaderConstants::mediumInvaderWidth) {

                    float newWidth;
                    if (invaders[i].getWidth() ==
                        InvaderConstants::largeInvaderWidth) {
                        newWidth = InvaderConstants::mediumInvaderWidth;
                    } else {
                        newWidth = InvaderConstants::smallInvaderWidth;
                    }

                    invaders.emplace_back(invaders[i].textureMap,
                                          invaders[i].posX - newWidth / 2.0f,
                                          invaders[i].posY + newWidth / 2.0f,
                                          newWidth);
                    invaders.emplace_back(invaders[i].textureMap,
                                          invaders[i].posX + newWidth / 2.0f,
                                          invaders[i].posY + newWidth / 2.0f,
                                          newWidth);
                    invaders.emplace_back(invaders[i].textureMap,
                                          invaders[i].posX - newWidth / 2.0f,
                                          invaders[i].posY - newWidth / 2.0f,
                                          newWidth);
                    invaders.emplace_back(invaders[i].textureMap,
                                          invaders[i].posX + newWidth / 2.0f,
                                          invaders[i].posY - newWidth / 2.0f,
                                          newWidth);
                }
            }
        }
    }

    invaders.erase(std::remove_if(invaders.begin(), invaders.end(),
                                  [&](const Invader& invader) {
                                      return invader.shouldDelete();
                                  }),
                   invaders.end());

    for (auto& invader : invaders) {
        invader.update(deltaTime, window);
    }
}
