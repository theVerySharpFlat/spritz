#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cglm/cam.h"
#include "renderer_internal.h"
#include "spritz/camera.h"
#include "spritz/window.h"
#include <spritz/renderer.h>
#include <window_internal.h>

void spritzRendererBegin(SpritzRenderer_t* renderer, SpritzWindow_t window,
                         SpritzCamera_t camera) {
    renderer->quadData.statistics.nQuads = 0;
    renderer->quadData.statistics.nBatches = 0;

    window->graphicsAPI.PFN_begin(window->graphicsAPI.internalData, camera);
}

void spritzRendererEnd(SpritzRenderer_t* renderer, SpritzWindow_t window) {
    spritzRendererFlushQuads(renderer, window);

    window->graphicsAPI.PFN_end(window->graphicsAPI.internalData);
}

SpritzRenderer_t spritzRendererInitialize(SpritzRendererOptions_t options) {
    SpritzRenderer_t renderer = {};

    renderer.options = options;

    renderer.quadData.indicies =
        malloc(sizeof(SpritzRendererQuadIndexSet_t) * options.nQuadsPerBatch *
               SPRITZ_RENDERER_NUM_INDICES_PER_QUAD);
    renderer.quadData.vertices =
        malloc(sizeof(SpritzRendererQuadVertex_t) * options.nQuadsPerBatch *
               SPRITZ_RENDERER_NUM_VERTICES_PER_QUAD);

    return renderer;
}

void spritzRendererDestruct(SpritzRenderer_t* renderer) {
    free(renderer->quadData.vertices);
    free(renderer->quadData.indicies);
}

void spritzRendererFlushQuads(SpritzRenderer_t* renderer,
                              SpritzWindow_t window) {
    if (renderer->quadData.nextQuadIndex <= 0) {
        return;
    }

    window->graphicsAPI.PFN_quadDrawCall(window->graphicsAPI.internalData,
                                         renderer);
    renderer->quadData.nextQuadIndex = 0;
    renderer->quadData.nextTextureIndex = 0;
}

void spritzRendererQueueQuad(SpritzRenderer_t* renderer, SpritzWindow_t window,
                             SpritzRendererQuadInfo_t quad) {
    if (renderer->quadData.nextQuadIndex >= renderer->options.nQuadsPerBatch ||
        renderer->quadData.nextTextureIndex >= 16) {
        spritzRendererFlushQuads(renderer, window);
    }

    // Make sure we don't load two instances of the same texture handle into the
    // drawcall's texture array

    uint32_t foundTextureIndex = renderer->quadData.nextQuadIndex;

    for (uint32_t i = 0; i < renderer->quadData.nextTextureIndex; i++) {
        if (renderer->quadData.textures[i] == quad.texture) {
            //printf("foundTexture\n");
            foundTextureIndex = i;
        }
    }

    if (foundTextureIndex == renderer->quadData.nextQuadIndex) {
        renderer->quadData.textures[renderer->quadData.nextTextureIndex] = quad.texture;
        renderer->quadData.nextTextureIndex++;
        // printf("did not find texture\n");
    }

    SpritzRendererQuadVertex_t v0 = {.posX = quad.topLeftX,
                                     .posY = quad.topLeftY,
                                     .texX = quad.texTopLeftX,
                                     .texY = quad.texTopLeftY,
                                     .colorR = quad.colorR,
                                     .colorG = quad.colorG,
                                     .colorB = quad.colorB,
                                     .texIndex = foundTextureIndex,
                                     .finalColorAlpha = quad.finalColorAlpha,
                                     .texAndColorBlendFactor =
                                         quad.texAndColorBlendFactor};

    SpritzRendererQuadVertex_t v1 = {.posX = quad.bottomRightX,
                                     .posY = quad.topLeftY,
                                     .texX = quad.texBottomRightX,
                                     .texY = quad.texTopLeftY,
                                     .colorR = quad.colorR,
                                     .colorG = quad.colorG,
                                     .colorB = quad.colorB,
                                     .texIndex = foundTextureIndex,
                                     .finalColorAlpha = quad.finalColorAlpha,
                                     .texAndColorBlendFactor =
                                         quad.texAndColorBlendFactor};

    SpritzRendererQuadVertex_t v2 = {.posX = quad.topLeftX,
                                     .posY = quad.bottomRightY,
                                     .texX = quad.texTopLeftX,
                                     .texY = quad.texBottomRightY,
                                     .colorR = quad.colorR,
                                     .colorG = quad.colorG,
                                     .colorB = quad.colorB,
                                     .texIndex = foundTextureIndex,
                                     .finalColorAlpha = quad.finalColorAlpha,
                                     .texAndColorBlendFactor =
                                         quad.texAndColorBlendFactor};

    SpritzRendererQuadVertex_t v3 = {.posX = quad.bottomRightX,
                                     .posY = quad.bottomRightY,
                                     .texX = quad.texBottomRightX,
                                     .texY = quad.texBottomRightY,
                                     .colorR = quad.colorR,
                                     .colorG = quad.colorG,
                                     .colorB = quad.colorB,
                                     .texIndex = foundTextureIndex,
                                     .finalColorAlpha = quad.finalColorAlpha,
                                     .texAndColorBlendFactor =
                                         quad.texAndColorBlendFactor};

    SpritzRendererQuadVertex_t* vPtr =
        (renderer->quadData.vertices + SPRITZ_RENDERER_NUM_VERTICES_PER_QUAD *
                                           renderer->quadData.nextQuadIndex);
    vPtr[0] = v0;
    vPtr[1] = v1;
    vPtr[2] = v2;
    vPtr[3] = v3;

    SpritzRendererQuadIndexSet_t* iPtr =
        (renderer->quadData.indicies + renderer->quadData.nextQuadIndex);
    memcpy(iPtr, &spritzRendererDefaultQuadIndexSet,
           sizeof(spritzRendererDefaultQuadIndexSet));

    for (int i = 0; i < SPRITZ_RENDERER_NUM_INDICES_PER_QUAD; i++) {
        iPtr->indices[i] += SPRITZ_RENDERER_NUM_VERTICES_PER_QUAD *
                            renderer->quadData.nextQuadIndex;
    }

    renderer->quadData.nextQuadIndex++;
}

void spritzRendererLoadTexture(SpritzRenderer_t* renderer,
                               SpritzWindow_t window,
                               SpritzRendererTextureCreateInfo_t createInfo,
                               SpritzRendererTextureHandle_t* texture) {
    window->graphicsAPI.PFN_loadTexture(window->graphicsAPI.internalData,
                                        createInfo, texture);
}

void spritzRendererFreeTexture(SpritzRenderer_t* renderer,
                               SpritzWindow_t window,
                               SpritzRendererTextureHandle_t texture) {
    window->graphicsAPI.PFN_freeTexture(window->graphicsAPI.internalData,
                                        texture);
}
