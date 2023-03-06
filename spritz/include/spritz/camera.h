#ifndef SPRITZ_CAMERA_H
#define SPRITZ_CAMERA_H

typedef struct {
    float projectionLeft, projectionRight, projectionTop, projectionBottom;
} SpritzCameraCreateInfo_t;

typedef struct SpritzCameraInternal* SpritzCamera_t;

SpritzCamera_t spritzCreateCamera(SpritzCameraCreateInfo_t createInfo);
void spritzDestroyCamera(SpritzCamera_t camera);

#endif
