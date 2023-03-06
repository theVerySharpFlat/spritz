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

        .projectionLeft = 0.0,
        .projectionRight = 720.0,
        .projectionTop = 0.0,
        .projectionBottom = 540.0,

        .rendererOptions = rendererOptions
    };

    SpritzWindow_t window =
        spritzWindowCreate(createInfo);

    SpritzRendererQuadInfo_t quadInfo= {
        NULL,
        -0.5f, 0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f
    };

    while (!spritzWindowShouldClose(window)) {
        spritzWindowSetClearColor(window, 0.0f, 0.0f, 0.0f, 1.0f);
        spritzWindowClear(window);

        spritzBegin(window);

        spritzQueueQuad(window, quadInfo);
        spritzFlush(window);

        spritzEnd(window);

        spritzWindowSwapBuffers(window);
        spritzUpdateWindows();
    }
}
