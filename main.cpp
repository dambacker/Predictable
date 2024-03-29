//
// gl-test1.cpp 
//

#include "pch.h"

// Include standard headers
#include <windows.h>
#include <stdio.h>

// Include GL headers
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "app.h"

//=============================================================================
// GLOBALS
//=============================================================================

App app;

struct 
{
    int width  = 1024;
    int height = 768;
} globals;

//-----------------------------------------------------------------------------
void WindowResizeCallback(GLFWwindow* window, int width, int height)
//-----------------------------------------------------------------------------
{
    globals.width  = width;
    globals.height = height;

    app.Scale(width, height);
}

//DEBUG CountBits experimental code
unsigned char CountBits1(unsigned char n)
{
    unsigned char count = 0;

    for (int i = 0; i < 8; i++)
    {
        if (n & (1 << i))
            count += 1;
    }

    return count;
}

unsigned char CountBits2(unsigned char n)
{
    unsigned char count = n & 1;
    while (n >>= 1)
    {
        count += n & 1;
    }
    return count;
}

unsigned char CountBits3(unsigned char n)
{
    unsigned char count = 0;
    while (n)
    {
        count++;
        //n & ~(n - 1)   returns the first set bit in n
        n -= n & ~(n - 1);
        //example, n = 6
        //n=0110
        //  0110 - 0110 & ~(0110 - 1)
        //  0110 - 0110 & ~(0101)
        //  0110 - 0110 & 1010
        //  0110 - 0010
        //  0100
    }
    return count;
}

void test()
{
    for (unsigned int n = 0; n <= 255; n++)
    {
        unsigned char c1 = CountBits1(n);
        unsigned char c2 = CountBits2(n);
        unsigned char c3 = CountBits3(n);

        fprintf(stderr, "CountBits(%3d) = %d\n", n, c1);

        if (c1 != c2)
        {
            fprintf(stderr, "ERROR: CountBits2(%d) = %d, should be %d.\n", n, c2, c1);
            fprintf(stderr, "<press enter>\n");
            getchar();
        }

        if (c1 != c3)
        {
            fprintf(stderr, "ERROR: CountBits3(%d) = %d, should be %d.\n", n, c3, c1);
            fprintf(stderr, "<press enter>\n");
            getchar();
        }
    }
    
    fprintf(stderr, "Done.\n");
    fprintf(stderr, "<press enter>\n");
    getchar();
}

//end of DEBUG CountBits

//-----------------------------------------------------------------------------
int main(void)
//-----------------------------------------------------------------------------
{
//    test();

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#if 0   //fullscreen
    glfwWindowHint(GLFW_MAXIMIZED, 1);
    glfwWindowHint(GLFW_DECORATED, 0);
#endif

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow(globals.width, globals.height, "Predictable", NULL, NULL);
    if (window == NULL) 
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeCallback(window, WindowResizeCallback);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) 
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwGetWindowSize(window, &globals.width, &globals.height);

    if (app.Init(globals.width, globals.height) != R_OK)
    {
        fprintf(stderr, "Failed to Initialize\n");
        getchar();
        glfwTerminate();
    }

    do 
    {
        //get window stats
        double curX = 0;
        double curY = 0;
        glfwGetCursorPos(window, &curX, &curY);
        int press = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);
        float time = (float)glfwGetTime();
        
        //return mouse pos as [0 .. 1]
        float mouseX = (float)(curX/globals.width);
        float mouseY = 1.0f - (float)(curY/globals.height);

        //update app and render
        app.Mouse(mouseX, mouseY, press);
        app.Render(time);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    app.Cleanup();

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

