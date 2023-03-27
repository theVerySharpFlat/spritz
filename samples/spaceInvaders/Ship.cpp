#include "Ship.h"
#include "CollisionUtil.h"
#include "WindowConstants.h"

#include <algorithm>
#include <cmath>
#include <forward_list>
#include <iostream>
#include <ostream>

Ship::Ship(SpritzRendererTextureHandle_t texture, float x, float y)
    : textureMap(texture) {
    posX = x;
    posY = y;
    bullets.reserve(maxBullets);
}

void Ship::update(double deltaTime, SpritzWindow_t window) {
    if (currentAnimationState == IDLE) {
        if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_A)) {
            setPosition(posX - horizontalSpeed * deltaTime, posY);
        }

        if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_D)) {
            setPosition(posX + horizontalSpeed * deltaTime, posY);
        }

        if ((posX + width / 2.0f) > (WindowConstants::width / 2.0f)) {
            setPosition((WindowConstants::width / 2.0f) - width / 2.0f, posY);
        }

        if ((posX - width / 2.0f) < (-WindowConstants::width / 2.0f)) {
            setPosition((-WindowConstants::width / 2.0f) + width / 2.0f, posY);
        }

        static double timeSinceLastFire = 0.0;
        if (bullets.size() < maxBullets &&
            spritzWindowIsKeyPressed(window, SPRITZ_KEY_ENTER) &&
            timeSinceLastFire > minFireDelay) {
            bullets.emplace_back(textureMap, posX, posY + height / 2.0f);
            timeSinceLastFire = std::fmod(timeSinceLastFire, minFireDelay);
        } else {
            timeSinceLastFire += deltaTime;
        }
    }

    if (!bullets.empty()) {
        bullets.erase(
            std::remove_if(
                bullets.begin(), bullets.end(),
                [&](const Bullet& bullet) {
                    CollisionUtil::Rect screenRect = {
                        .x = 0.0,
                        .y = 0.0,
                        .width = WindowConstants::width,
                        .height = WindowConstants::height};
                    return bullet.shouldDelete() ||
                           !(CollisionUtil::rectCollidesWithRect(
                                 screenRect, bullet.getCollisionRect()) ||
                             CollisionUtil::rectCollidesWithRect(
                                 bullet.getCollisionRect(), screenRect));
                }),
            bullets.end());
    }

    for (auto& bullet : bullets) {
        bullet.update(deltaTime, window);
    }
}

void Ship::setPosition(float x, float y) {
    posX = x;
    posY = y;
}

SpritzRendererQuadInfo_t Ship::getQuad() const {
    const AnimationFrame& currentAnimationFrame =
        idleAnimationFrames[currentAnimationFrameIndex];
    return {.texture = textureMap,
            .topLeftX = posX - width / 2.0f,
            .topLeftY = posY + height / 2.0f,
            .bottomRightX = posX + width / 2.0f,
            .bottomRightY = posY - height / 2.0f,
            .texTopLeftX = currentAnimationFrame.left,
            .texTopLeftY = currentAnimationFrame.top,
            .texBottomRightX = currentAnimationFrame.right,
            .texBottomRightY = currentAnimationFrame.bottom,
            .colorR = 1.0f,
            .colorG = 1.0f,
            .colorB = 1.0f,
            .finalColorAlpha = 1.0f,
            .texAndColorBlendFactor = 1.0f};
}

CollisionUtil::Rect Ship::getCollisionRect() const {
    return {.x = posX,
            .y = posY,
            .width = (12.0f / 16.0f) * width,
            .height = (14.0f / 16.0f) * height};
}
