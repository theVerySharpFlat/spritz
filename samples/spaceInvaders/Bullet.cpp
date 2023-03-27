#include "Bullet.h"
#include "CollisionUtil.h"
#include "TextureMapMeta.h"
#include "spritz/renderer.h"

Bullet::Bullet(SpritzRendererTextureHandle_t texture, float x, float y)
    : textureMap(texture) {
    posX = x;
    posY = y;
}


void Bullet::setPosition(float x, float y) {
    posX = x;
    posY = y;
}

void Bullet::update(double deltaTime, SpritzWindow_t window) {
    setPosition(posX, posY + deltaTime * verticalSpeed);
}

SpritzRendererQuadInfo_t Bullet::getQuad() const {
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

CollisionUtil::Rect Bullet::getCollisionRect() const {
    return {.x = posX,
            .y = posY,
            .width = (2.0f / 16.0f) * width,
            .height = (4.0f / 16.0f) * height};
}
