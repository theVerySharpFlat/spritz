#include "spritz/camera.h"
#include "camera_internal.h"
#include "cglm/cam.h"
#include "cglm/types.h"

static void updateViewMatrix(SpritzCamera_t camera) {
    vec3 eye = {camera->position[0], camera->position[1], 0.5f};
    vec3 center = {camera->position[0], camera->position[1], 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_lookat(eye, center, up, camera->view);
}

SpritzCamera_t spritzCameraCreate(SpritzCameraCreateInfo_t createInfo) {
    SpritzCameraInternal_t* camera = malloc(sizeof(SpritzCameraInternal_t));

    glm_ortho(createInfo.projectionLeft, createInfo.projectionRight,
              createInfo.projectionBottom, createInfo.projectionTop, 1.0f,
              -1.0f, camera->projection);

    camera->position[0] = 0.0f;
    camera->position[1] = 0.0f;

    updateViewMatrix(camera);

    return camera;
}

void spritzCameraSetPosition(SpritzCamera_t camera, float x, float y) {
    camera->position[0] = x;
    camera->position[1] = y;
    updateViewMatrix(camera);
}

void spritzCameraGetPosition(SpritzCamera_t camera, float* x, float* y) {
    *x = camera->position[0];
    *y = camera->position[1];
}

void spritzCameraDestroy(SpritzCamera_t camera) { free(camera); }
