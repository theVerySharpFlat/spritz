#ifndef SPRITZ_GRAPHICS_API_INTERNAL_H
#define SPRITZ_GRAPHICS_API_INTERNAL_H

#include "renderer_internal.h"
#include "spritz/camera.h"
#include "spritz/graphics_api.h"
#include "spritz/renderer.h"
#include "spritz/window.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint32_t nQuadsPerBatch;

    SpritzWindow_t window;
} SpritzGraphicsAPIInitInfo_t;

typedef bool (*SpritzGraphicsAPIPreWindowSystemInitFN)(void* uData);
typedef bool (*SpritzGraphicsAPIInitFN)(void* uData,
                                        SpritzGraphicsAPIInitInfo_t);
typedef bool (*SpritzGraphicsAPIShutdownFN)(void* uData);

typedef bool (*SpritzGraphicsAPIClearFN)(void* uData);
typedef bool (*SpritzGraphicsAPISetClearFN)(void* uData, float r, float g,
                                            float b, float a);

typedef bool (*SpritzGraphicsAPIQuadDrawCallFN)(void* uData,
                                                SpritzRenderer_t* renderer);

typedef bool (*SpritzGraphicsAPIViewportResizeFN)(void* uData, int offX,
                                                  int offY, int width,
                                                  int height);

typedef bool (*SpritzGraphicsAPILoadTextureFN)(
    void* uData, SpritzRendererTextureCreateInfo_t createInfo,
    SpritzRendererTextureHandle_t* texture);

typedef bool (*SpritzGraphicsAPIFreeTextureFN)(
    void* uData, SpritzRendererTextureHandle_t texture);

typedef bool (*SpritzGraphicsAPIBeginFN)(void* uData, SpritzCamera_t camera);
typedef bool (*SpritzGraphicsAPIEndFN)(void* uData);

typedef struct {
    SpritzGraphicsAPIPreWindowSystemInitFN PFN_preInit;
    SpritzGraphicsAPIInitFN PFN_init;
    SpritzGraphicsAPIShutdownFN PFN_shutdown;

    SpritzGraphicsAPIClearFN PFN_clear;
    SpritzGraphicsAPISetClearFN PFN_setClearColor;

    SpritzGraphicsAPILoadTextureFN PFN_loadTexture;
    SpritzGraphicsAPIFreeTextureFN PFN_freeTexture;

    SpritzGraphicsAPIQuadDrawCallFN PFN_quadDrawCall;

    SpritzGraphicsAPIViewportResizeFN PFN_viewportResize;

    SpritzGraphicsAPIBeginFN PFN_begin;
    SpritzGraphicsAPIEndFN PFN_end;

    void* internalData;
} SpritzGraphicsAPIInternal_t;

SpritzGraphicsAPIInternal_t
spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference);

#endif
