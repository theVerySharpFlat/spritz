#include "graphics_api_gl_shaders.h"

const char* spritzGlQuadShaderVS =
    "#version 410 core\n"
    "layout (location = 0) in vec2 iPos;\n"
    "layout (location = 1) in int iTexIndex;\n"
    "layout (location = 2) in vec2 iTexCoords;\n"
    "layout (location = 3) in vec4 iColor;\n"
    "layout (location = 4) in float iTexAndColorBlendFactor;\n"
    "\n"
    "out vec4 color;\n"
    "\n"
    "uniform mat4 uView;\n"
    "\n"
    "uniform mat4 uProj;\n"
    "\n"
    "void main() {\n"
    "   gl_Position = uView * uProj * vec4(iPos, 0.0, 1.0);\n"
    "   color = iColor;"
    "}\n";

const char* spritzGlQuadShaderFS =
    "#version 410 core\n"
    "\n"
    "in vec4 color;\n"
    "\n"
    "out vec4 oColor;\n"
    "\n"
    "void main() {\n"
    "   oColor = vec4(1.0, 0.0, 0.0, 1.0);//color;\n"
    "}\n"
    ;
