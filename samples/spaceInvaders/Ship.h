#ifndef SPACE_INVADERS_SHIP_H
#define SPACE_INVADERS_SHIP_H

#include "AnimationFrame.h"
#include "Bullet.h"
#include "GameObject.h"
#include "TextureMapMeta.h"
#include "spritz/spritz.h"
#include <vector>

class Ship : public GameObject {
public:
    Ship(SpritzRendererTextureHandle_t textureMap, float x, float y);

    void setPosition(float x, float y);

    float getX() const override { return posX; }
    float getY() const override { return posY; }

    float getWidth() const override { return width; }
    float getHeight() const override { return height; }

    void update(double deltaTime, SpritzWindow_t window) override;

    CollisionUtil::Rect getCollisionRect() const override;

    SpritzRendererQuadInfo_t getQuad() const override;

    std::vector<Bullet>& getBullets() { return bullets; }

    enum AnimationState { IDLE = 0 };

  friend class InvaderManager;

  private:
    inline static const AnimationFrame idleAnimationFrames[1] = {{
        .left = 0.0f / TextureMapMeta::width,
        .right = 16.0f / TextureMapMeta::width,
        .top = 32.0f / TextureMapMeta::height,
        .bottom = 16.0f / TextureMapMeta::height,
    }};
    inline static const int nIdleAnimationFrames = 1;

    const SpritzRendererTextureHandle_t textureMap;

    float posX, posY;

    inline static const float width = 50.0f, height = 50.0f;

    AnimationState currentAnimationState = IDLE;
    int currentAnimationFrameIndex = 0;

    float horizontalSpeed = 0.2;

    const int maxBullets = 8;
    const double minFireDelay = 200.0;
    std::vector<Bullet> bullets;
};

#endif
