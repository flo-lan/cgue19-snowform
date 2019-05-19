#pragma once

struct GLFWwindow;

class Screen
{
public:
    static void SetWindow(GLFWwindow* window);

    static int GetWidth() { return width; }
    static int GetHeight() { return height; }

private:
    static int width;  // The current width of the screen window in pixels
    static int height; // The current height of the screen window in pixels
};
