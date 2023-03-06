#ifndef SPRITZ_RENDERER_API_INTERNAL_H
#define SPRITZ_RENDERER_API_INTERNAL_H

#include "spritz/camera.h"
#include <spritz/renderer.h>
#include <spritz/window.h>
#include <stdint.h>

#include <cglm/types.h>
#include <cglm/cglm.h>

typedef struct {
    float posX;
    float posY;

    int32_t texIndex;
    float texX;
    float texY;

    float colorR;
    float colorG;
    float colorB;
    float finalColorAlpha;

    float texAndColorBlendFactor;
} SpritzRendererQuadVertex_t;

#define SPRITZ_RENDERER_NUM_VERTICES_PER_QUAD 4
#define SPRITZ_RENDERER_NUM_INDICES_PER_QUAD 6

typedef struct {
    uint32_t indices[SPRITZ_RENDERER_NUM_INDICES_PER_QUAD];
} SpritzRendererQuadIndexSet_t;

static SpritzRendererQuadIndexSet_t spritzRendererDefaultQuadIndexSet = {
    {0, 1, 2, 2, 1, 3}};

typedef enum {
    SpritzRendererVertexLayoutTypeNull = 0,
    SpritzRendererVertexLayoutTypeFloat32,
    SpritzRendererVertexLayoutTypeInt32
} SpritzRendererVertexLayoutType_t;

typedef struct {
    SpritzRendererVertexLayoutType_t type;
    uint32_t count;
} SpritzRendererVertexLayoutEntry_t;

const static SpritzRendererVertexLayoutEntry_t
    spritzRendererQuadVertexLayout[] = {
        {SpritzRendererVertexLayoutTypeFloat32, 2}, // pos
        {SpritzRendererVertexLayoutTypeInt32, 1},   // texIndex
        {SpritzRendererVertexLayoutTypeFloat32, 2}, // texCoords
        {SpritzRendererVertexLayoutTypeFloat32, 4}, // color
        {SpritzRendererVertexLayoutTypeFloat32, 1}, // texAndColorBlendFactor
        {SpritzRendererVertexLayoutTypeNull, 1}     // NULL terminator
};

typedef struct {
    void* textureHandle;
} SpritzRendererTexture;

typedef struct {
    SpritzRendererOptions_t options;
    struct {
        SpritzRendererQuadVertex_t* vertices;
        SpritzRendererQuadIndexSet_t* indicies;
        uint32_t nextQuadIndex;

        struct {
            uint32_t nBatches;
            uint32_t nQuads;
        } statistics;
    } quadData;

    mat4 projection;
} SpritzRenderer_t;

SpritzRenderer_t spritzRendererInitialize(SpritzRendererOptions_t options);

void spritzRendererDestruct(SpritzRenderer_t* renderer);

void spritzRendererQueueQuad(SpritzRenderer_t* renderer, SpritzWindow_t window,
                             SpritzRendererQuadInfo_t quad);

void spritzRendererFlushQuads(SpritzRenderer_t* renderer,
                              SpritzWindow_t window);

void spritzRendererBegin(SpritzRenderer_t* renderer, SpritzWindow_t window, SpritzCamera_t camera);

void spritzRendererEnd(SpritzRenderer_t* renderer, SpritzWindow_t window);

#endif // !SPRITZ_RENDERER_API_INTERNAL_H
