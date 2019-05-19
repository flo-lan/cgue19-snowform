#include "Screen.h"
#include <GLFW/glfw3.h>

int Screen::width = 0;
int Screen::height = 0;

void Screen::SetWindow(GLFWwindow* window)
{
    if (!window)
    {
        return;
    }

    glfwGetWindowSize(window, &width, &height);
}
