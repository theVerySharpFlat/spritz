#include "spritz_graphics_api.h"

#include "spritz_graphics_api_gl.h"

SpritzGraphicsAPIInternal_t spritzLoadGraphicsAPI(SpritzGraphicsAPIInitInfo_t initInfo) {
    return spritzGraphicsAPIGLLoad();
}
