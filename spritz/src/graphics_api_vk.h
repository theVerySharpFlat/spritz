#ifndef SPRITZ_GRAPHICS_API_VK_H
#define SPRITZ_GRAPHICS_API_VK_H

#include "volk.h"
#include "graphics_api_internal.h"

SpritzGraphicsAPIInternal_t spritzGraphicsAPIVKLoad();

bool spritzGraphicsAPIVKPreWindowSystemInit(void* uData);

#endif