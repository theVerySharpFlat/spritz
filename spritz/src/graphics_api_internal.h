#ifndef SPRITZ_GRAPHICS_API_INTERNAL_H
#define SPRITZ_GRAPHICS_API_INTERNAL_H

#include "renderer_internal.h"
#include "spritz/camera.h"
#include "spritz/graphics_api.h"
#include "spritz/window.h"

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

typedef bool (*SpritzGraphicsAPIQuadDrawCallFN)(void*,
                                                SpritzRenderer_t* renderer);
typedef bool (*SpritzGraphicsAPIBeginFN)(void*, SpritzCamera_t camera);
typedef bool (*SpritzGraphicsAPIEndFN)(void*);

typedef struct {
    SpritzGraphicsAPIPreWindowSystemInitFN PFN_preInit;
    SpritzGraphicsAPIInitFN PFN_init;
    SpritzGraphicsAPIShutdownFN PFN_shutdown;

    SpritzGraphicsAPIClearFN PFN_clear;
    SpritzGraphicsAPISetClearFN PFN_setClearColor;

    SpritzGraphicsAPIQuadDrawCallFN PFN_quadDrawCall;

    SpritzGraphicsAPIBeginFN PFN_begin;
    SpritzGraphicsAPIEndFN PFN_end;

    void* internalData;
} SpritzGraphicsAPIInternal_t;

SpritzGraphicsAPIInternal_t
spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference);

#endif
