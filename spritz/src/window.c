#include "GLFW/glfw3.h"
#include "graphics_api_internal.h"
#include <spritz/graphics_api.h>

#include <stdio.h>
#include <stdlib.h>

#include "renderer_internal.h"
#include "spritz/camera.h"
#include "spritz/renderer.h"
#include "window_internal.h"
#include <spritz/window.h>

static int windowCount = 0;

static void GLFWErrorCallback(int error, const char* description) {
    printf("GLFW erorr (%d): %s", error, description);
}

static void GLFWFramebufferResizeCallback(GLFWwindow* window, int width,
                                          int height) {
    SpritzWindow_t spritzWindow = glfwGetWindowUserPointer(window);

    // calculate expanded dimensions
    float expandedWidth =
        (float)height * spritzWindow->aspectRatio; // what would the width be if
                                                   // we maximized the height?
    float expandedHeight =
        (float)width / spritzWindow->aspectRatio; // what would the height be if
                                                  // we maximized the width?

    bool maximizeWidth = (int)expandedHeight <= height;
    bool maximizeHeight = (int)expandedWidth <= width;

    int viewportWidth = 0, viewportHeight = 0;
    if (maximizeWidth) {
        viewportWidth = width;
        viewportHeight = expandedHeight;
    } else if (maximizeHeight) {
        viewportWidth = expandedWidth;
        viewportHeight = height;
    } else {
        fprintf(stderr,
                "spritz: There should never be this point in window resizing "
                "where we can't decide whether or not to maximize width or "
                "maximize height! This is a developer bug!\n");
        return;
    }

    int offsetX = 0, offsetY = 0;
    offsetX = (int)((float)(width - viewportWidth) / 2.0f);
    offsetY = (int)((float)(height - viewportHeight) / 2.0f);

    if (offsetX < 0 || offsetY < 0) {
        fprintf(stderr, "Window viewport offset should never be less than "
                        "zero. This is a developer bug!\n");
        return;
    }

    spritzWindow->graphicsAPI.PFN_viewportResize(spritzWindow->graphicsAPI.internalData, offsetX, offsetY, viewportWidth, viewportHeight);
}

static void GLFWWIndowResizeCallback(GLFWwindow* window, int width, int height) {
    GLFWFramebufferResizeCallback(window, width * 2, height * 2);
}

SpritzWindow_t spritzWindowCreate(SpritzWindowCreateInfo_t createInfo) {
    SpritzWindow_t window = malloc(sizeof(struct SpritzWindow));

    if (!windowCount) {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW!\n");
        }
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    window->aspectRatio = (float)createInfo.width / (float)createInfo.height;

    SpritzGraphicsAPIInternal_t graphicsAPI =
        spritzLoadGraphicsAPI(createInfo.apiPreference);

    graphicsAPI.PFN_preInit(graphicsAPI.internalData);

    GLFWwindow* nativeWindow = glfwCreateWindow(
        createInfo.width, createInfo.height, createInfo.name, NULL, NULL);
    glfwMakeContextCurrent(nativeWindow);

    window->window = nativeWindow;
    window->graphicsAPI = graphicsAPI;

    glfwSetWindowUserPointer(nativeWindow, window);
    glfwSetFramebufferSizeCallback(nativeWindow, GLFWFramebufferResizeCallback);
    glfwSetWindowSizeCallback(nativeWindow, GLFWWIndowResizeCallback);

    SpritzGraphicsAPIInitInfo_t initInfo = {};
    initInfo.window = window;
    initInfo.nQuadsPerBatch = 128;

    graphicsAPI.PFN_init(graphicsAPI.internalData, initInfo);

    window->renderer = spritzRendererInitialize(createInfo.rendererOptions);

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
    return window->graphicsAPI.PFN_setClearColor(
        window->graphicsAPI.internalData, r, g, b, a);
}

bool spritzQueueQuad(SpritzWindow_t window, SpritzRendererQuadInfo_t quadInfo) {
    spritzRendererQueueQuad(&window->renderer, window, quadInfo);
    return true;
}

bool spritzFlush(SpritzWindow_t window) {
    spritzRendererFlushQuads(&window->renderer, window);
    return true;
}

bool spritzBegin(SpritzWindow_t window, SpritzCamera_t camera) {
    spritzRendererBegin(&window->renderer, window, camera);
    return true;
}

bool spritzEnd(SpritzWindow_t window) {
    spritzRendererEnd(&window->renderer, window);
    return true;
}
