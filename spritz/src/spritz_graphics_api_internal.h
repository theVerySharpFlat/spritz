#ifndef SPRITZ_GRAPHICS_API_INTERNAL_H
#define SPRITZ_GRAPHICS_API_INTERNAL_H

#include "spritz_graphics_api.h"
#include "spritz_window.h"
#include <stdbool.h>
#include <stdint.h>


struct SpritzGraphicsAPIInitInfo {
    uint32_t nQuadsPerBatch;

    SpritzWindow_t window;
};

typedef struct SpritzGraphicsAPIInitInfo SpritzGraphicsAPIInitInfo_t;

typedef bool (*SpritzGraphicsAPIPreWindowSystemInitFN)(SpritzGraphicsAPIInitInfo_t);
typedef bool (*SpritzGraphicsAPIInitFN)(SpritzGraphicsAPIInitInfo_t);
typedef bool (*SpritzGraphicsAPIShutdownFN)(void);

typedef bool (*SpritzGraphicsAPIClearFN)(void);
typedef bool (*SpritzGraphicsAPISetClearFN)(float r, float g, float b, float a);

struct SpritzGraphicsAPI  {
    SpritzGraphicsAPIPreWindowSystemInitFN PFN_preInit;
    SpritzGraphicsAPIInitFN PFN_init;
    SpritzGraphicsAPIShutdownFN PFN_shutdown;

    SpritzGraphicsAPIClearFN PFN_clear;
    SpritzGraphicsAPISetClearFN PFN_setClearColor;

    void* internalData;
};
typedef struct SpritzGraphicsAPI SpritzGraphicsAPIInternal_t;

SpritzGraphicsAPIInternal_t spritzLoadGraphicsAPI(SpritzGraphicsAPIID_t preference);

#endif
