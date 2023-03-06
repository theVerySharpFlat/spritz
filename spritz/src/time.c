#include "spritz/time.h"
#include "GLFW/glfw3.h"

double spritzGetTime() {
    return glfwGetTime() * 1000.0f;
}
