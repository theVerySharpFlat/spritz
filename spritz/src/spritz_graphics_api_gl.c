#include "spritz_graphics_api_gl.h"
#include "spritz_graphics_api.h"
#include <OpenGL/OpenGL.h>
#include <stdio.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <spritz_window_internal.h>

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

bool spritzGraphicsAPIGLPreWindowSystemInit(SpritzGraphicsAPIInitInfo_t info) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int* num = NULL;
    return true;
}

bool spritzGraphicsAPIGLInit(SpritzGraphicsAPIInitInfo_t info) {
    glfwMakeContextCurrent(info.window->window);

    if (!gladLoadGL()) {
        fprintf(stderr, "spritz: Failed to initialize OpenGL!");
        return false;
    }

    return true;
}

bool spritzGraphicsAPIGLClear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
    return true;
}

bool spritzGraphicsAPIGLSetClearColor(float r, float g, float b, float a) {
    GLCall(glClearColor(r, g, b, a));
    return true;
}

SpritzGraphicsAPI_t spritzGraphicsAPIGLLoad() {
    SpritzGraphicsAPI_t api = {};
    api.PFN_init = spritzGraphicsAPIGLInit;
    api.PFN_preInit = spritzGraphicsAPIGLPreWindowSystemInit;
    api.PFN_clear = spritzGraphicsAPIGLClear;
    api.PFN_shutdown = spritzGraphicsAPIGLShutdown;
    api.PFN_setClearColor = spritzGraphicsAPIGLSetClearColor;

    return api;
}
