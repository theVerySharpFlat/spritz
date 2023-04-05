#include "Invader.h"
#include <cmath>
#include <iostream>
#include <ostream>

Invader::Invader(SpritzRendererTextureHandle_t textureHandle, float posX,
                 float posY, float width)
    : textureMap(textureHandle) {
    this->posX = posX;
    this->posY = posY;
    this->width = width;
    this->height = width;
}

void Invader::setPosition(float x, float y) {
    posX = x;
    posY = y;
}

void Invader::update(double deltaTime, SpritzWindow_t window) {
    timeSinceAnimSwitch += deltaTime;

    if (currentAnimationFrameIndex >= 2) {
        frameIncDirection = -1;
    }

    if (currentAnimationFrameIndex <= 0) {
        frameIncDirection = 1;
    }

    if (currentAnimationState == IDLE) {
        if (timeSinceAnimSwitch > msPerIdleAnimFrameSwitch) {
            currentAnimationFrameIndex += frameIncDirection;
        }

        timeSinceAnimSwitch =
            ::fmodf(timeSinceAnimSwitch, msPerIdleAnimFrameSwitch);

        posY += verticalSpeed;
    }
}

SpritzRendererQuadInfo_t Invader::getQuad() const {
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

CollisionUtil::Rect Invader::getCollisionRect() const {
    return {.x = getX(),
            .y = getY(),
            .width = (12.0f / 16.0f) * width,
            .height = (14.0f / 16.0f) * height};
}
