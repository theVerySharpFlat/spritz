#include "graphics_api_internal.h"
#include "graphics_api_gl.h"

SpritzGraphicsAPIInternal_t spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference) {
    return spritzGraphicsAPIGLLoad();
}
