#ifndef SPACE_INVADERS_INVADER_H
#define SPACE_INVADERS_INVADER_H

#include "GameObject.h"
#include "spritz/renderer.h"
#include "spritz/spritz.h"

#include "AnimationFrame.h"
#include "TextureMapMeta.h"

namespace InvaderConstants {
    const float largeInvaderWidth = 200.0f;
    const float mediumInvaderWidth = 100.0f;
    const float smallInvaderWidth = 50.0f;
}

class Invader : public GameObject {
  public:
    Invader(SpritzRendererTextureHandle_t textureMap, float x, float y, float width);

    void setPosition(float x, float y);

    float getX() const override { return posX; }
    float getY() const override { return posY; }

    float getWidth() const override { return width; }
    float getHeight() const override { return height; }

    void update(double deltaTime, SpritzWindow_t window) override;

    CollisionUtil::Rect getCollisionRect() const override;

    SpritzRendererQuadInfo_t getQuad() const override;

    enum AnimationState { IDLE = 0 };

    friend class InvaderManager;

  private:
    inline const static AnimationFrame idleAnimationFrames[3] = {
        {
            .left = 0.0f / TextureMapMeta::width,
            .right = 16.0f / TextureMapMeta::width,
            .top = 1.0f,
            .bottom = 48.0f / TextureMapMeta::height,
        },
        {
            .left = 16.0f / TextureMapMeta::width,
            .right = 32.0f / TextureMapMeta::width,
            .top = 1.0f,
            .bottom = 48.0f / TextureMapMeta::height,
        },
        {
            .left = 32.0f / TextureMapMeta::width,
            .right = 48.0f / TextureMapMeta::width,
            .top = 1.0f,
            .bottom = 48.0f / TextureMapMeta::height,
        }};
    inline const static int nIdleAnimationFrames = 3;


    SpritzRendererTextureHandle_t textureMap;

    float posX, posY;

    float width = 50.0f, height = 50.0f;

    AnimationState currentAnimationState = IDLE;
    int currentAnimationFrameIndex = 0;
    int frameIncDirection = 1;

    inline const static double msPerIdleAnimFrameSwitch = 300;
    double timeSinceAnimSwitch = 0.0;

    inline const static float verticalSpeed = -0.4f;
};

#endif
