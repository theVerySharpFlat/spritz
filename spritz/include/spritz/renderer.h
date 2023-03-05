#ifndef SPRITZ_RENDERER_H
#define SPRITZ_RENDERER_H

#include <stdint.h>

typedef struct {
    uint32_t nQuadsPerBatch;
} SpritzRendererOptions_t;

typedef struct SpritzRendererTexture* SpritzRendererTexture_t;

typedef struct {
    SpritzRendererTexture_t texture;

    float topLeftX;
    float topLeftY;
    float bottomRightX;
    float bottomRightY;

    float texTopLeftX;
    float texTopLeftY;
    float texBottomRightX;
    float texBottomRightY;

    float colorR;
    float colorG;
    float colorB;
    float finalColorAlpha;

    float texAndColorBlendFactor;
} SpritzRendererQuadInfo_t;

#endif
