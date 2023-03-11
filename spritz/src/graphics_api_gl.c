#include "graphics_api_gl.h"
#include "camera_internal.h"
#include "graphics_api_gl_shaders.h"
#include "graphics_api_internal.h"
#include "spritz/camera.h"
#include <OpenGL/OpenGL.h>
#include <renderer_internal.h>
#include <spritz/graphics_api.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "window_internal.h"

#include <glad/glad.h>

#define GLCall(x)                                                              \
    GlClearError();                                                            \
    x;                                                                         \
    if (GLGetErrors(#x, __FILE__, __LINE__))                                   \
        return false;

void GlClearError() {
    while (glGetError() != GL_NO_ERROR)
        ;
}

bool GLGetErrors(const char* function, const char* file, int line) {
    GLenum error = glGetError();
    bool errorOccured = false;
    while (error != GL_NO_ERROR) {
        errorOccured = true;
        const char* errorType;
        switch (error) {
        case GL_INVALID_ENUM:
            errorType = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            errorType = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorType = "GL_INVALID_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            errorType = "GL_OUT_OF_MEMORY";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorType = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        default:
            errorType = "UNKNOWN_ERROR";
            break;
        }

        fprintf(stderr,
                "spritz:  OPENGL ERROR %X (\"%s\") in %s at %s on line %d\n",
                error, errorType, function, file, line);
        error = glGetError();
    }
    return errorOccured;
}

typedef struct {
    const char* name;
    const char* data;
    uint32_t size;
} ShaderUnitSource_t;

typedef struct {
    const ShaderUnitSource_t vs;
    const ShaderUnitSource_t fs;
} SpritzShaderProgramSource_t;

static bool spritzCompileShader(uint32_t* shader, GLenum type,
                                ShaderUnitSource_t source) {
    GLCall(*shader = glCreateShader(type));
    GLCall(glShaderSource(*shader, 1, &source.data, NULL));
    GLCall(glCompileShader(*shader));

    int32_t success;
    char infoLog[512];

    GLCall(glGetShaderiv(*shader, GL_COMPILE_STATUS, &success));
    if (!success) {
        GLCall(glGetShaderInfoLog(*shader, 512, NULL, infoLog));
        fprintf(stderr, "SHADER COMPILATION ERROR (type = %#X)\n", (int)type);
        fprintf(stderr, "LOG: \n{%s}\n", infoLog);
        return false;
    }

    return true;
}

static bool spritzLinkProgram(const uint32_t* program) {
    GLCall(glLinkProgram(*program));

    int32_t success;
    char infoLog[512];

    GLCall(glGetProgramiv(*program, GL_LINK_STATUS, &success));
    if (!success) {
        GLCall(glGetProgramInfoLog(*program, 512, NULL, infoLog));
        fprintf(stderr, "SHADER LINKING FAILED: \n");
        fprintf(stderr, "LOG: \n{%s}\n", infoLog);
        fprintf(stderr, "failed to compile shader\n");
        return false;
    }

    return true;
}

static uint32_t spritzCreateShader(const SpritzShaderProgramSource_t source) {
    uint32_t id;
    GLCall(id = glCreateProgram());

    uint32_t vertexShader;
    uint32_t fragmentShader;

    spritzCompileShader(&vertexShader, GL_VERTEX_SHADER, source.vs);
    spritzCompileShader(&fragmentShader, GL_FRAGMENT_SHADER, source.fs);

    GLCall(glAttachShader(id, vertexShader));
    GLCall(glAttachShader(id, fragmentShader));
    spritzLinkProgram(&id);

    GLCall(glDeleteShader(vertexShader));
    GLCall(glDeleteShader(fragmentShader));

    return id;
}

bool spritzGraphicsAPIGLPreWindowSystemInit(void* apiData) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    return true;
}

bool spritzGraphicsAPIGLInit(void* apiData, SpritzGraphicsAPIInitInfo_t info) {
    SpritzGLInternalData_t* glData = apiData;

    glfwMakeContextCurrent(info.window->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "spritz: Failed to initialize OpenGL!\n");
        return false;
    }

    spritzGraphicsAPIGLCreateVertexBuffer(
        apiData, spritzRendererQuadVertexLayout, &(glData->quadVBO));

    SpritzShaderProgramSource_t source = {
        {"spritzGlQuadShaderVS", spritzGlQuadShaderVS,
         strlen(spritzGlQuadShaderVS)},
        {"spritzGlQuadShaderFS", spritzGlQuadShaderFS,
         strlen(spritzGlQuadShaderFS)}};
    glData->quadShader.id = spritzCreateShader(source);

    return true;
}

bool spritzGraphicsAPIGLClear(void* apiData) {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    return true;
}

bool spritzGraphicsAPIGLSetClearColor(void* apiData, float r, float g, float b,
                                      float a) {
    GLCall(glClearColor(r, g, b, a));
    return true;
}

bool spritzGraphicsAPIGLShutdown(void* apiData) {
    SpritzGLInternalData_t* glData = apiData;

    spritzGraphicsAPIGLFreeVertexBuffer(glData, &glData->quadVBO);

    GLCall(glDeleteShader(glData->quadShader.id));

    free(apiData);
    return true;
}

bool spritzGraphicsAPIGLCreateVertexBuffer(
    void* apiData, const SpritzRendererVertexLayoutEntry_t* layoutArray,
    SpritzGLVBO_t* vbo) {
    GLCall(glGenBuffers(1, &(vbo->vboID)));
    GLCall(glGenBuffers(1, &(vbo->iboID)));
    GLCall(glGenVertexArrays(1, &(vbo->vaoID)));

    GLCall(glBindVertexArray(vbo->vaoID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->vboID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->iboID));

    int stride = 0;
    for (int i = 0; layoutArray[i].type != SpritzRendererVertexLayoutTypeNull;
         i++) {
        stride += 4 * layoutArray[i].count;
    }

    uint64_t offset = 0;
    for (int i = 0; layoutArray[i].type != SpritzRendererVertexLayoutTypeNull;
         i++) {
        if (layoutArray[i].type == SpritzRendererVertexLayoutTypeFloat32) {
            GLCall(glVertexAttribPointer(i, layoutArray[i].count, GL_FLOAT,
                                         GL_FALSE, stride, (void*)offset));
        } else if (layoutArray[i].type == SpritzRendererVertexLayoutTypeInt32) {
            GLCall(glVertexAttribIPointer(i, layoutArray[i].count, GL_INT,
                                          stride, (void*)offset));
        }

        GLCall(glEnableVertexAttribArray(i));
        offset += 4 * (uint64_t)layoutArray[i].count;
    }

    vbo->maxSize = 0;
    vbo->maxElements = 0;

    vbo->nElements = 0;
    vbo->size = 0;

    return true;
}

bool spritzGraphicsAPIGLFreeVertexBuffer(void* apiData, SpritzGLVBO_t* vbo) {
    GLCall(glDeleteBuffers(1, &vbo->iboID));
    GLCall(glDeleteBuffers(1, &vbo->vboID));
    GLCall(glDeleteVertexArrays(1, &vbo->vaoID));

    return true;
}

bool spritzGraphicsAPIGLSetVBOData(void* apiData, SpritzGLVBO_t* vbo,
                                   void* data, uint32_t size) {
    GLCall(glBindVertexArray(vbo->vaoID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo->vboID));

    if (size > vbo->maxSize) {
        GLCall(glBufferData(GL_ARRAY_BUFFER, (int)size, data, GL_DYNAMIC_DRAW));
        vbo->maxSize = size;
        vbo->size = size;
    } else {
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, (int)size, data));
        vbo->size = size;
    }

    return true;
}

bool spritzGraphicsAPIGLSetIBOData(void* apiData, SpritzGLVBO_t* vbo,
                                   uint32_t* data, uint32_t nElements) {
    GLCall(glBindVertexArray(vbo->vaoID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->iboID));

    if (nElements > vbo->maxElements) {
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                            (int)(nElements * sizeof(uint32_t)), data,
                            GL_DYNAMIC_DRAW));
        vbo->maxElements = nElements;
        vbo->nElements = nElements;
    } else {
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                               nElements * sizeof(uint32_t), data));
        vbo->nElements = nElements;
    }

    return true;
}

bool spritzGraphicsAPIGLQuadDrawCMD(void* apiData, SpritzRenderer_t* renderer) {
    SpritzGLInternalData_t* glData = apiData;

    GLCall(glBindVertexArray(glData->quadVBO.vaoID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, glData->quadVBO.vboID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData->quadVBO.iboID));

    spritzGraphicsAPIGLSetVBOData(
        apiData, &glData->quadVBO, renderer->quadData.vertices,
        renderer->quadData.nextQuadIndex * sizeof(SpritzRendererQuadVertex_t) *
            SPRITZ_RENDERER_NUM_VERTICES_PER_QUAD);
    spritzGraphicsAPIGLSetIBOData(apiData, &glData->quadVBO,
                                  (uint32_t*)renderer->quadData.indicies,
                                  renderer->quadData.nextQuadIndex *
                                      SPRITZ_RENDERER_NUM_INDICES_PER_QUAD);

    GLCall(glUseProgram(glData->quadShader.id));

    GLCall(glDrawElements(GL_TRIANGLES,
                          renderer->quadData.nextQuadIndex *
                              SPRITZ_RENDERER_NUM_INDICES_PER_QUAD,
                          GL_UNSIGNED_INT, NULL));

    return true;
}

bool spritzGraphicsAPIGLBegin(void* apiData, SpritzCamera_t camera) {
    SpritzGLInternalData_t* glData = apiData;

    GLCall(glUseProgram(glData->quadShader.id));

    if (glData->quadShaderUViewLocation < 0) {
        GLCall(glData->quadShaderUViewLocation =
                   glGetUniformLocation(glData->quadShader.id, "uView"));
    }

    if (glData->quadShaderUProjLocation < 0) {
        GLCall(glData->quadShaderUProjLocation =
                   glGetUniformLocation(glData->quadShader.id, "uProj"));
    }

    GLCall(glUniformMatrix4fv(glData->quadShaderUProjLocation, 1, GL_FALSE,
                              camera->projection[0]));
    GLCall(glUniformMatrix4fv(glData->quadShaderUViewLocation, 1, GL_FALSE,
                              camera->view[0]));

    return true;
}

bool spritzGraphicsAPIGLEnd(void* apiData) { return true; }

bool spritzGraphicsAPIGLViewportResize(void *apiData, int offX, int offY, int width, int height) {
    printf("hereeee\n");
    GLCall(glViewport(offX, offY, width, height));
    return true;
}

SpritzGraphicsAPIInternal_t spritzGraphicsAPIGLLoad() {
    SpritzGraphicsAPIInternal_t api = {};
    api.PFN_init = spritzGraphicsAPIGLInit;
    api.PFN_preInit = spritzGraphicsAPIGLPreWindowSystemInit;
    api.PFN_clear = spritzGraphicsAPIGLClear;
    api.PFN_shutdown = spritzGraphicsAPIGLShutdown;
    api.PFN_setClearColor = spritzGraphicsAPIGLSetClearColor;
    api.PFN_quadDrawCall = spritzGraphicsAPIGLQuadDrawCMD;
    api.PFN_begin = spritzGraphicsAPIGLBegin;
    api.PFN_end = spritzGraphicsAPIGLEnd;
    api.PFN_viewportResize = spritzGraphicsAPIGLViewportResize;

    api.internalData = malloc(sizeof(SpritzGLInternalData_t));

    SpritzGLInternalData_t* glData = api.internalData;
    glData->quadShaderUProjLocation = -1;
    glData->quadShaderUViewLocation = -1;

    return api;
}
