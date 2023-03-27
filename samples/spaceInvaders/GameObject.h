#ifndef SPACE_INVADERS_GAME_OBJECT_H
#define SPACE_INVADERS_GAME_OBJECT_H

#include "CollisionUtil.h"
#include <spritz/spritz.h>

#include <iostream>

class GameObject {
public:
    virtual void update(double deltaTime, SpritzWindow_t window) = 0;
    virtual SpritzRendererQuadInfo_t getQuad() const = 0;
    
    virtual float getWidth() const = 0;
    virtual float getHeight() const = 0;

    virtual float getX() const = 0;
    virtual float getY() const = 0;

    void setShouldDelete(bool del) { _shouldDelete = del; }
    bool shouldDelete() const { return _shouldDelete; }

    virtual CollisionUtil::Rect getCollisionRect() const = 0;

    bool collidesWith(const GameObject& other) const {
        CollisionUtil::Rect us = getCollisionRect();
        CollisionUtil::Rect them = other.getCollisionRect();

        return CollisionUtil::rectCollidesWithRect(us, them) || CollisionUtil::rectCollidesWithRect(them, us);
    }

private:
    bool _shouldDelete = false;
};

#endif
