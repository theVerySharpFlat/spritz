#ifndef SPRITZ_GRAPHICS_API_INTERNAL_H
#define SPRITZ_GRAPHICS_API_INTERNAL_H

#include "spritz/graphics_api.h"
#include "spritz/window.h"
#include "renderer_internal.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t nQuadsPerBatch;

    SpritzWindow_t window;
} SpritzGraphicsAPIInitInfo_t;

typedef bool (*SpritzGraphicsAPIPreWindowSystemInitFN)(void*);
typedef bool (*SpritzGraphicsAPIInitFN)(void*, SpritzGraphicsAPIInitInfo_t);
typedef bool (*SpritzGraphicsAPIShutdownFN)(void*);

typedef bool (*SpritzGraphicsAPIClearFN)(void*);
typedef bool (*SpritzGraphicsAPISetClearFN)(void*, float r, float g, float b,
                                            float a);

typedef bool (*SpritzGraphicsQuadDrawCallFN)(void*, SpritzRenderer_t* renderer);

typedef struct {
    SpritzGraphicsAPIPreWindowSystemInitFN PFN_preInit;
    SpritzGraphicsAPIInitFN PFN_init;
    SpritzGraphicsAPIShutdownFN PFN_shutdown;

    SpritzGraphicsAPIClearFN PFN_clear;
    SpritzGraphicsAPISetClearFN PFN_setClearColor;

    SpritzGraphicsQuadDrawCallFN PFN_quadDrawCall;

    void* internalData;
} SpritzGraphicsAPIInternal_t;

SpritzGraphicsAPIInternal_t
spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference);

#endif
