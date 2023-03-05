#ifndef SPRITZ_WINDOW_INTERNAL_H
#define SPRITZ_WINDOW_INTERNAL_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "renderer_internal.h"

#include <spritz/graphics_api.h>
#include "graphics_api_internal.h"

struct SpritzWindow {
    GLFWwindow* window;
    SpritzGraphicsAPIInternal_t graphicsAPI;
    SpritzRenderer_t renderer;
};

#endif
