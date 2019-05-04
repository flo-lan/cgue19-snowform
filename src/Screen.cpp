#include "Screen.h"

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
