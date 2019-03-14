#pragma once

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <unordered_map>

class InputManager
{
public:
    // https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
    static InputManager& getInstance()
    {
        static InputManager instance;
        return instance;
    }

    void SetMouseScrollValues(float x, float y)
    {
        mouseScrollValues.x = x;
        mouseScrollValues.y = y;
    }

    void SetMousePosition(float x, float y)
    {
        mousePosition.x = x;
        mousePosition.y = y;
    }

    void SetMouseButtonPressedState(int button, bool pressed)
    {
        mouseButtonPressed[button] = pressed;
    }

    float GetMouseScrollValueX() const { return mouseScrollValues.x; }
    float GetMouseScrollValueY() const { return mouseScrollValues.y; }

    glm::vec2 GetMousePosition() const { return mousePosition; }
    float GetMousePositionX() const { return mousePosition.x; }
    float GetMousePositionY() const { return mousePosition.y; }

    bool IsLeftMouseButtonPressed() const;
    bool IsRightMouseButtonPressed() const;

private:
    InputManager();

    glm::vec2 mouseScrollValues;
    glm::vec2 mousePosition;
    std::unordered_map<int /* Button */, bool> mouseButtonPressed;
};

#define sInputManager InputManager::getInstance()

static void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    sInputManager.SetMouseScrollValues(sInputManager.GetMouseScrollValueX() + (float)xoffset, sInputManager.GetMouseScrollValueY() + (float)yoffset);
}

static void MouseCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    sInputManager.SetMousePosition((float)xpos, (float)ypos);
}

static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    sInputManager.SetMouseButtonPressedState(button, action == GLFW_PRESS);
}
