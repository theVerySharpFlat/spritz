#include "GLFW/glfw3.h"
#include <spritz/graphics_api.h>
#include "graphics_api_internal.h"

#include <stdio.h>
#include <stdlib.h>

#include <spritz/window.h>
#include "renderer_internal.h"
#include "spritz/renderer.h"
#include "window_internal.h"

static int windowCount = 0;

static void GLFWErrorCallback(int error, const char* description) {
    printf("GLFW erorr (%d): %s", error, description);
}

SpritzWindow_t spritzWindowCreate(int32_t width, int32_t height,
                                  const char* name,
                                  SpritzGraphicsAPIID_t apiPreference) {
    SpritzWindow_t window = malloc(sizeof(struct SpritzWindow));

    if (!windowCount) {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW!\n");
        }
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    SpritzGraphicsAPIInternal_t graphicsAPI =
        spritzLoadGraphicsAPI(apiPreference);

    graphicsAPI.PFN_preInit(graphicsAPI.internalData);

    GLFWwindow* nativeWindow =
        glfwCreateWindow(width, height, name, NULL, NULL);
    glfwMakeContextCurrent(nativeWindow);

    window->window = nativeWindow;
    window->graphicsAPI = graphicsAPI;

    SpritzGraphicsAPIInitInfo_t initInfo = {};
    initInfo.window = window;
    initInfo.nQuadsPerBatch = 128;

    graphicsAPI.PFN_init(graphicsAPI.internalData, initInfo);

    SpritzRendererOptions_t rendererOptions = {
        128
    };

    window->renderer = spritzRendererInitialize(rendererOptions);

    windowCount++;

    return window;
}

void spritzWindowDestroy(SpritzWindow_t* window) {
    (*window)->graphicsAPI.PFN_shutdown((*window)->graphicsAPI.internalData);
    glfwDestroyWindow((*window)->window);
    free(*window);

    windowCount--;
}

void spritzUpdateWindows(void) { glfwPollEvents(); }

void spritzWindowSwapBuffers(SpritzWindow_t window) {
    glfwSwapBuffers(window->window);
}

bool spritzWindowShouldClose(SpritzWindow_t window) {
    return (bool)glfwWindowShouldClose(window->window);
}

bool spritzWindowIsKeyPressed(SpritzWindow_t window, int32_t key) {
    int32_t state = glfwGetKey(window->window, key);
    return state == GLFW_REPEAT || state == GLFW_PRESS;
}

bool spritzWindowClear(SpritzWindow_t window) {
    return window->graphicsAPI.PFN_clear(window->graphicsAPI.internalData);
}

bool spritzWindowSetClearColor(SpritzWindow_t window, float r, float g, float b,
                               float a) {
    return window->graphicsAPI.PFN_setClearColor(window->graphicsAPI.internalData, r, g, b, a);
}

bool spritzQueueQuad(SpritzWindow_t window, SpritzRendererQuadInfo_t quadInfo) {
    spritzRendererQueueQuad(&window->renderer, window, quadInfo);
    return true;
}

bool spritzFlush(SpritzWindow_t window) {
    spritzRendererFlushQuads(&window->renderer, window);
    return true;
}
