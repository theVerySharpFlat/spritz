#ifndef SPRITZ_GRAPHICS_API_H
#define SPRITZ_GRAPHICS_API_H

#include "spritz_window.h"
#include <stdbool.h>
#include <stdint.h>

enum SpritzGraphicsAPIID {
    SpritzGraphicsAPIGL = 0
};
typedef enum SpritzGraphicsAPIID SpritzGraphicsAPIID_t;

struct SpritzGraphicsAPIInitInfo {
    SpritzGraphicsAPIID_t graphicsAPIPreference;
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

SpritzGraphicsAPIInternal_t spritzLoadGraphicsAPI(SpritzGraphicsAPIInitInfo_t initInfo);

#endif
