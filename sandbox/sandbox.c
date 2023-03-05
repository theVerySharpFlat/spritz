#include "spritz/renderer.h"
#include <stdio.h>

#include <spritz/spritz.h>
#include <spritz/window.h>

int main() {
    SpritzWindow_t window =
        spritzWindowCreate(680, 420, "Spritz!", SpritzGraphicsAPIGL);

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

        spritzQueueQuad(window, quadInfo);
        spritzFlush(window);

        spritzWindowSwapBuffers(window);
        spritzUpdateWindows();
    }
}
