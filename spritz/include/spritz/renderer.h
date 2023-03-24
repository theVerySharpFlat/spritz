#ifndef SPRITZ_RENDERER_H
#define SPRITZ_RENDERER_H

#include <stdint.h>

typedef struct {
    uint32_t nQuadsPerBatch;
} SpritzRendererOptions_t;

typedef enum {
    SpritzRendererImageRGB = 3,
    SpritzRendererImageRGBA = 4
} SpritzRendererImageType;

typedef struct {
    int width, height;
    SpritzRendererImageType imageType;

    const uint8_t* data;
    uint32_t size;
} SpritzRendererTextureCreateInfo_t;

typedef void* SpritzRendererTextureHandle_t;

typedef struct {
    SpritzRendererTextureHandle_t texture;

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
