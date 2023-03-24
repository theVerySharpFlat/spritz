#include "spritz/camera.h"
#include "spritz/time.h"
#include <stdio.h>

#include <spritz/graphics_api.h>
#include <spritz/renderer.h>
#include <spritz/spritz.h>
#include <spritz/window.h>

#include "stb_image.h"

int main() {
    SpritzRendererOptions_t rendererOptions = {.nQuadsPerBatch = 1024};

    SpritzWindowCreateInfo_t createInfo = {.width = 720,
                                           .height = 540,

                                           .apiPreference = SpritzGraphicsAPIGL,
                                           .name = "Spritz!",

                                           .rendererOptions = rendererOptions};

    SpritzWindow_t window = spritzWindowCreate(createInfo);

    SpritzCameraCreateInfo_t cameraCreateInfo = {-360.0f, 360.0f, 270.0f,
                                                 -270.0f};
    SpritzCamera_t camera = spritzCameraCreate(cameraCreateInfo);

    int width, height, nChannels;
    void* iData =  stbi_load("assets/test.png", &width, &height, &nChannels, 3);
    SpritzRendererTextureHandle_t textureHandle;

    SpritzRendererTextureCreateInfo_t textureCreateInfo = {
        .width = width,
        .height = height,
        .data = iData,
        .size = sizeof(width * height * nChannels),
        .imageType = SpritzRendererImageRGB
    };

    spritzLoadTexture(window, textureCreateInfo, &textureHandle);

    uint64_t frameNumber = 0;
    double timeTotal = 0;
    while (!spritzWindowShouldClose(window)) {
        double start = spritzGetTime();

        spritzWindowSetClearColor(window, 0.0f, 0.0f, 0.0f, 1.0f);
        spritzWindowClear(window);

        {
            float camX, camY;
            spritzCameraGetPosition(camera, &camX, &camY);

            if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_W)) {
                camY += 1.5;
            }
            if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_A)) {
                camX += -1.5;
            }
            if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_S)) {
                camY += -1.5;
            }
            if (spritzWindowIsKeyPressed(window, SPRITZ_KEY_D)) {
                camX += 1.5;
            }

            spritzCameraSetPosition(camera, camX, camY);
        }

        spritzBegin(window, camera);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                float left = i * 110.0f;
                float bottom = j * 110.0f;
                SpritzRendererQuadInfo_t quadInfo = {
                    .topLeftX = left,
                    .bottomRightX = left + 100.0f,
                    .topLeftY = bottom + 100.0f,
                    .bottomRightY = bottom,
                    .colorR = 0.0f,
                    .colorG = 1.0f,
                    .colorB = 0.0f,
                    .texture = textureHandle,
                    .texTopLeftX = 0.0f,
                    .texTopLeftY = 1.0f,
                    .texBottomRightX = 1.0f,
                    .texBottomRightY = 0.0f
                };
                spritzQueueQuad(window, quadInfo);
            }
        }

        spritzEnd(window);

        spritzWindowSwapBuffers(window);
        spritzUpdateWindows();

        timeTotal += spritzGetTime() - start;
        if (frameNumber % 10 == 0) {
            printf("average frame time: %f\n", timeTotal / 10.0);
            timeTotal = 0.0;
        }

        frameNumber++;
    }
}
