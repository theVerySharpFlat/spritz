#include "spritz/camera.h"
#include <stdio.h>

#include <spritz/graphics_api.h>
#include <spritz/renderer.h>
#include <spritz/spritz.h>
#include <spritz/window.h>

int main() {
    SpritzRendererOptions_t rendererOptions = {
        .nQuadsPerBatch = 128
    };

    SpritzWindowCreateInfo_t createInfo = {
        .width = 720,
        .height = 540,
        
        .apiPreference = SpritzGraphicsAPIGL,
        .name = "Spritz!",

        .rendererOptions = rendererOptions
    };

    SpritzWindow_t window =
        spritzWindowCreate(createInfo);

    SpritzCameraCreateInfo_t cameraCreateInfo = {
        -360.0f, 360.0f,
        270.0f, -270.0f
    };
    SpritzCamera_t camera = spritzCreateCamera(cameraCreateInfo);

    SpritzRendererQuadInfo_t quadInfo= {
        NULL,
        100.0f, 200.0f, 200.0f, 100.0f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f
    };

    while (!spritzWindowShouldClose(window)) {
        spritzWindowSetClearColor(window, 0.0f, 0.0f, 0.0f, 1.0f);
        spritzWindowClear(window);

        spritzBegin(window, camera);

        spritzQueueQuad(window, quadInfo);
        spritzFlush(window);

        spritzEnd(window);

        spritzWindowSwapBuffers(window);
        spritzUpdateWindows();
    }
}
