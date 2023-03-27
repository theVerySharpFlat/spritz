#ifndef SPACE_INVADERS_BULLET_H
#define SPACE_INVADERS_BULLET_H

#include "GameObject.h"
#include "AnimationFrame.h"
#include "TextureMapMeta.h"

#include <spritz/spritz.h>

class Bullet : public GameObject {
public:
    Bullet(SpritzRendererTextureHandle_t textureMap, float x, float y);

    void setPosition(float x, float y);

    float getX() const override { return posX; }
    float getY() const override { return posY; }

    float getWidth() const override { return width; }
    float getHeight() const override { return height; }

    void update(double deltaTime, SpritzWindow_t window) override;

    CollisionUtil::Rect getCollisionRect() const override;

    SpritzRendererQuadInfo_t getQuad() const override;

    enum AnimationState { IDLE = 0 };

  private:
    inline const static AnimationFrame idleAnimationFrames[1] = {{
        .left = 48.0f / TextureMapMeta::width,
        .right = 1.0f,
        .top = 32.0f / TextureMapMeta::height,
        .bottom = 16.0f / TextureMapMeta::height,
    }};
    inline const static int nIdleAnimationFrames = 1;

    SpritzRendererTextureHandle_t textureMap;

    float posX, posY;

    inline static const float width = 50.0f, height = 50.0f;

    AnimationState currentAnimationState = IDLE;
    int currentAnimationFrameIndex = 0;

    float verticalSpeed = 0.4;
};

#endif
