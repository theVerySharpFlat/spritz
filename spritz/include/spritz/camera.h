#ifndef SPRITZ_CAMERA_H
#define SPRITZ_CAMERA_H

typedef struct {
    float projectionLeft, projectionRight, projectionTop, projectionBottom;
} SpritzCameraCreateInfo_t;

typedef struct SpritzCameraInternal* SpritzCamera_t;

SpritzCamera_t spritzCameraCreate(SpritzCameraCreateInfo_t createInfo);
void spritzCameraDestroy(SpritzCamera_t camera);

void spritzCameraSetPosition(SpritzCamera_t camera, float x, float y);
void spritzCameraGetPosition(SpritzCamera_t camera, float* x, float* y);

#endif
