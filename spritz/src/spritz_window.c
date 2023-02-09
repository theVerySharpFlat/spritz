#include "GLFW/glfw3.h"

#include <stdlib.h>
#include <stdio.h>

#include <spritz_window.h>
#include <spritz_window_internal.h>

static int windowCount = 0;

SpritzWindow_t spritzCreateWindow(int32_t width, int32_t height, const char* name) {
    if(!windowCount) {
        if(!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW!\n");
        }
    }

    GLFWwindow* nativeWindow = glfwCreateWindow(width, height, name, NULL, NULL);

    SpritzWindow_t window = malloc(sizeof(struct SpritzWindow));

    window->window = nativeWindow;

    
    windowCount++;

    return window;
}


void spritzDestroyWindow(SpritzWindow_t *window) {
    glfwDestroyWindow((*window)->window);
    free(*window);

    windowCount--;
}

void spritzUpdateWindows(void) {
    glfwPollEvents();
}

bool spritzWindowShouldClose(SpritzWindow_t window) {
    return (bool)glfwWindowShouldClose(window->window);
}

bool spritzIsKeyPressed(SpritzWindow_t window, int32_t key) {
    int32_t state = glfwGetKey(window->window, key);
    return state == GLFW_REPEAT || state == GLFW_PRESS;
}
