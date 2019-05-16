#include "InputManager.h"
#include <GLFW\glfw3.h>

InputManager::InputManager() :
    mouseScrollValues(0.f, 0.f),
    mousePosition(0.f, 0.f)
{
}

void InputManager::Initialize(GLFWwindow* window)
{
    double mousePositionX = 0.f;
    double mousePositionY = 0.f;

    glfwGetCursorPos(window, &mousePositionX, &mousePositionY);

    mousePosition.x = (float)mousePositionX;
    mousePosition.y = (float)mousePositionY;
}

bool InputManager::IsLeftMouseButtonPressed() const
{
    std::unordered_map<int, bool>::const_iterator itr = mouseButtonPressed.find(GLFW_MOUSE_BUTTON_LEFT);
    return itr != mouseButtonPressed.end() && itr->second;
}

bool InputManager::IsRightMouseButtonPressed() const
{
    std::unordered_map<int, bool>::const_iterator itr = mouseButtonPressed.find(GLFW_MOUSE_BUTTON_RIGHT);
    return itr != mouseButtonPressed.end() && itr->second;
}

bool InputManager::IsKeyPressed(int key) const
{
    std::unordered_map<int, bool>::const_iterator itr = keyPressed.find(key);
    return itr != keyPressed.end() && itr->second;
}
