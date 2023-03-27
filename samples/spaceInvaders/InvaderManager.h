#ifndef SPACE_INVADERS_INVADER_MANAGER_H
#define SPACE_INVADERS_INVADER_MANAGER_H

#include "Invader.h"
#include "Ship.h"

#include <spritz/spritz.h>
#include <vector>

class InvaderManager {
  public:
    InvaderManager() {}

    void spawnLargeInvader(SpritzRendererTextureHandle_t textureHandle, float x,
                           float y);

    void spawnNLargeInvaders(SpritzRendererTextureHandle_t textureHandle, int n);

    void update(double deltaTime, SpritzWindow_t window, Ship& ship);

    const std::vector<Invader>& getInvaders() { return invaders; }

  private:
    std::vector<Invader> invaders;
};

#endif
