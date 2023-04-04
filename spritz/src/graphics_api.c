#include "spritz/graphics_api.h"
#include "graphics_api_gl.h"
#include "graphics_api_internal.h"
#include "graphics_api_vk.h"

SpritzGraphicsAPIInternal_t
spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference) {
    if (preference == SPRITZ_GRAPHICS_API_GL) {
        return spritzGraphicsAPIGLLoad();
    } else if (preference == SPRITZ_GRAPHICS_API_VK) {
        return spritzGraphicsAPIVKLoad();
    } else {
        SpritzGraphicsAPIInternal_t empty = {};
        printf("invalid graphics API preference!\n");
        return empty;
    }
}
