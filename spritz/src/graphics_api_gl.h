#ifndef SPRITZ_GRAPHICS_API_GL_H
#define SPRITZ_GRAPHICS_API_GL_H

#include "graphics_api_internal.h"
#include "renderer_internal.h"

struct SpritzGLVBO {
    uint32_t vaoID;
    uint32_t vboID;
    uint32_t iboID;

    uint32_t maxSize;
    uint32_t size;

    uint32_t maxElements;
    uint32_t nElements;
};
typedef struct SpritzGLVBO SpritzGLVBO_t;

struct SpritzGLShader {
    uint32_t id;
};
typedef struct SpritzGLShader SpritzGLShader_t;

typedef struct {
    SpritzGLShader_t quadShader;
    int quadShaderUProjLocation;
    int quadShaderUViewLocation;

    SpritzGLVBO_t quadVBO;
} SpritzGLInternalData_t;

bool spritzGraphicsAPIGLPreWindowSystemInit(void* apiData);
bool spritzGraphicsAPIGLInit(void* apiData, SpritzGraphicsAPIInitInfo_t info);

bool spritzGraphicsAPIGLShutdown(void* apiData);

bool spritzGraphicsAPIGLClear(void* apiData);
bool spritzGraphicsAPIGLSetClearColor(void* apiData, float r, float g, float b,
                                      float a);

SpritzGraphicsAPIInternal_t spritzGraphicsAPIGLLoad();

bool spritzGraphicsAPIGLCreateVertexBuffer(
    void* apiData, const SpritzRendererVertexLayoutEntry_t* layoutArray,
    SpritzGLVBO_t* vbo);
bool spritzGraphicsAPIGLSetVBOData(void* apiData, SpritzGLVBO_t* vbo,
                                   void* data, uint32_t size);
bool spritzGraphicsAPIGLSetIBOData(void* apiData, SpritzGLVBO_t* vbo,
                                   uint32_t* data, uint32_t nElements);
bool spritzGraphicsAPIGLFreeVertexBuffer(void* apiData, SpritzGLVBO_t* vbo);

bool spritzGraphicsAPIGLQuadDrawCMD(void* apiData, SpritzRenderer_t* renderer);

#endif
