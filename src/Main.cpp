/*
* Copyright 2018 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/


#include "Utils.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "ShaderProgram.h"
#include "SceneManager.h"
#include "GameScene.h"
#include "Settings.h"


/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */



/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */


/* --------------------------------------------- */
// Error Callback
/* --------------------------------------------- */

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

/* --------------------------------------------- */
// Input Callback
/* --------------------------------------------- */

bool gEnableWireframe = false;
bool gDisableCullFace = false;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        if (gEnableWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            gEnableWireframe = false;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            gEnableWireframe = true;
        }
    }

    if (key == GLFW_KEY_F2 && action == GLFW_PRESS)
    {
        if (!gDisableCullFace)
        {
            glDisable(GL_CULL_FACE);
            gDisableCullFace = true;
        }
        else
        {
            glEnable(GL_CULL_FACE);
            gDisableCullFace = false;
        }
    }
}

/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
    /* --------------------------------------------- */
    // Load settings.ini
    /* --------------------------------------------- */

	sSettings.Load();

    // load values from ini file
  	int width = sSettings.getWindowWidth();
    int height = sSettings.getWindowHeight();
    int refresh_rate = sSettings.getMaxFps();
    std::string window_title = sSettings.getTitle();

    /* --------------------------------------------- */
    // Init GLFW, create window and init GLEW
    /* --------------------------------------------- */

    // One of the few GLFW functions that may be called before initialization
    glfwSetErrorCallback(error_callback);

    // Before you can use most GLFW functions, the library must be initialized
    if (!glfwInit())
    {
        // Initialization failed
        EXIT_WITH_ERROR("Failed to init GLFW")
    }

    // The minimum required OpenGL version for this lecture is 4.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Newer versions may also be used.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    // Additionally, the context should be a core context, which marks the old fixed-function pipeline and its methods as deprecated
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_REFRESH_RATE, refresh_rate);
    //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Enable antialiasing (4xMSAA)
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(width, height, window_title.c_str(), NULL, NULL);
    if (!window)
    {
        // Window or OpenGL context creation failed
        EXIT_WITH_ERROR("Failed to create window or OpenGL context")
    }

    // Before we can use the OpenGL API, we must have a current OpenGL context
    // The context will remain current until you make another context current or until the window owning the current context is destroyed
    glfwMakeContextCurrent(window);

    // Register per-window callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, MouseScrollCallback);
    glfwSetCursorPosCallback(window, MouseCursorPositionCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

    // To force GLEW to load all functions, the variable glewExperimental has to be modified
    glewExperimental = true;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        EXIT_WITH_ERROR("Failed to init GLEW, something is seriously wrong")
    }

    fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    /* --------------------------------------------- */
    // Init framework
    /* --------------------------------------------- */

    if (!initFramework())
    {
        EXIT_WITH_ERROR("Failed to init framework")
    }

    /* --------------------------------------------- */
    // Initialize scene and render loop
    /* --------------------------------------------- */

    if (!sAssetManager.Load())
    {
        sAssetManager.Unload();

        EXIT_WITH_ERROR("Failed to load assets!")
    }

    if (!sSceneManager.LoadSceneFromFile<GameScene>("assets/scenes/game_scene.xml"))
    {
        EXIT_WITH_ERROR("Failed to load game scene!")
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        sSceneManager.Update();
        sSceneManager.Render();

        glfwSwapBuffers(window);
    }

    sSceneManager.Unload();
    sAssetManager.Unload();

    /* --------------------------------------------- */
    // Destroy framework
    /* --------------------------------------------- */

    destroyFramework();

    /* --------------------------------------------- */
    // Destroy context and exit
    /* --------------------------------------------- */

    // When a window and context is no longer needed, it should be destroyed
    // Once this function is called, no more events will be delivered for that window and its handle becomes invalid
    glfwDestroyWindow(window);

    // Just before the application exits, we need to terminate GLFW
    glfwTerminate();

    return EXIT_SUCCESS;
}




