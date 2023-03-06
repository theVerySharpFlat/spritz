#ifndef SPRITZ_CAMERA_INTERNAL_H
#define SPRITZ_CAMERA_INTERNAL_H

#include "spritz/camera.h"

#include <cglm/types.h>
#include <cglm/cglm.h>

struct SpritzCameraInternal {
    mat4 projection;
    mat4 view;

    vec2 position;
};

typedef struct SpritzCameraInternal SpritzCameraInternal_t;

#endif
