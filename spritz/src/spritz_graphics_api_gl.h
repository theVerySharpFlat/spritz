#ifndef SPRITZ_GRAPHICS_API_GL_H
#define SPRITZ_GRAPHICS_API_GL_H

#include "spritz_graphics_api.h"

bool spritzGraphicsAPIGLPreWindowSystemInit(SpritzGraphicsAPIInitInfo_t info);
bool spritzGraphicsAPIGLInit(SpritzGraphicsAPIInitInfo_t info);

inline bool spritzGraphicsAPIGLShutdown() { return true; };

bool spritzGraphicsAPIGLClear();
bool spritzGraphicsAPIGLSetClearColor(float r, float g, float b, float a);

SpritzGraphicsAPIInternal_t spritzGraphicsAPIGLLoad();

#endif
