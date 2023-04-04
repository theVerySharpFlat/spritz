#ifndef SPRITZ_GRAPHICS_API_H
#define SPRITZ_GRAPHICS_API_H

typedef struct SpritzGraphicsAPIInternal_t* SpritzGraphicsAPI_t;

enum SpritzGraphicsAPIID {
    SPRITZ_GRAPHICS_API_GL = 0,
    SPRITZ_GRAPHICS_API_VK
};
typedef enum SpritzGraphicsAPIID SpritzGraphicsAPIID_t;

#endif
