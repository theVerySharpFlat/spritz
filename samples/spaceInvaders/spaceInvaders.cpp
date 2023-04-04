#include "Invader.h"
#include "InvaderManager.h"
#include "Ship.h"
#include "WindowConstants.h"
#include "spritz/spritz.h"
#include "spritz/window.h"
#include "stb_image.h"
#include <iostream>

int main() {
    std::srand(std::time(nullptr));
    SpritzRendererOptions_t rendererOptions = {.nQuadsPerBatch = 1024};

    SpritzWindowCreateInfo_t createInfo = {
        .width = (int32_t)WindowConstants::width,
        .height = (int32_t)WindowConstants::height,
        .name = "Spritz!",
        .apiPreference = SPRITZ_GRAPHICS_API_GL,
        .rendererOptions = rendererOptions};

    SpritzWindow_t window = spritzWindowCreate(createInfo);

    SpritzCameraCreateInfo_t cameraCreateInfo = {-360.0f, 360.0f, 270.0f,
                                                 -270.0f};
    SpritzCamera_t camera = spritzCameraCreate(cameraCreateInfo);

    stbi_set_flip_vertically_on_load(1);
    int width, height, nChannels;
    uint8_t* iData = stbi_load("assets/spaceInvaders/spaceInvaders.png", &width,
                               &height, &nChannels, 4);
    if (iData == nullptr) {
        std::cout << "failed to load texture map!" << std::endl;
    }
    SpritzRendererTextureHandle_t textureHandle;

    SpritzRendererTextureCreateInfo_t textureCreateInfo = {
        .width = width,
        .height = height,
        .imageType = SpritzRendererImageRGBA,
        .data = iData,
        .size = sizeof(width * height * nChannels),
    };

    spritzLoadTexture(window, textureCreateInfo, &textureHandle);

    stbi_image_free(iData);

    // Invader invader = Invader(textureHandle, 0.0f, 0.0f, 200.0f);
    InvaderManager invaderManager;
    invaderManager.spawnLargeInvader(textureHandle, 0.0f, WindowConstants::height / 2.0f);
    Ship ship = Ship(textureHandle, 0.0f, -200.0);

    double deltaTime = 0;
    while (!spritzWindowShouldClose(window)) {
        double start = spritzGetTime();

        spritzWindowSetClearColor(window, 0.0f, 0.0f, 0.0f, 1.0f);
        spritzWindowClear(window);

        spritzBegin(window, camera);

        invaderManager.update(deltaTime, window, ship);
        for (auto& invader : invaderManager.getInvaders()) {
            spritzQueueQuad(window, invader.getQuad());
        }

        ship.update(deltaTime, window);
        spritzQueueQuad(window, ship.getQuad());

        for (auto& bullet : ship.getBullets()) {
            spritzQueueQuad(window, bullet.getQuad());
        }

        spritzEnd(window);

        spritzWindowSwapBuffers(window);
        spritzUpdateWindows();

        deltaTime = spritzGetTime() - start;
    }
}
