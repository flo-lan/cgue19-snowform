#include "InputManager.h"

InputManager::InputManager() :
    mouseScrollValues(0.f, 0.f),
    mousePosition(0.f, 0.f)
{
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
