#include "spritz_graphics_api.h"

#include "spritz_graphics_api_gl.h"

SpritzGraphicsAPI_t spritzLoadGraphicsAPI(SpritzGraphicsAPIInitInfo_t initInfo) {
    return spritzGraphicsAPIGLLoad();
}
