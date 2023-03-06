#include "spritz/camera.h"
#include "camera_internal.h"
#include "cglm/cam.h"
#include "cglm/types.h"

SpritzCamera_t spritzCreateCamera(SpritzCameraCreateInfo_t createInfo) {
    SpritzCameraInternal_t* camera = malloc(sizeof(SpritzCameraInternal_t));

    glm_ortho(createInfo.projectionLeft, createInfo.projectionRight,
              createInfo.projectionBottom, createInfo.projectionTop, -1.0f,
              1.0f, camera->projection);

    vec3 eye = {0.0f, 0.0f, -0.5f};
    vec3 center = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};
    glm_lookat(eye, center, up, camera->view);

    return camera;
}

void spritzDestroyCamera(SpritzCamera_t camera) {
    free(camera);
}
