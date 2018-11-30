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

#include "object.h"
#include "app.h"

//-----------------------------------------------------------------------------
int main(void)
//-----------------------------------------------------------------------------
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#if 0	//fullscreen
	glfwWindowHint(GLFW_MAXIMIZED, 1);
	glfwWindowHint(GLFW_DECORATED, 0);
#endif

	// Open a window and create its OpenGL context
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Predictable", NULL, NULL);
	if (window == NULL) 
	{
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	int width = 0;
	int height = 0;
	glfwGetWindowSize(window, &width, &height);

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

	App app;

	if (app.Init() == 0)
	{
		fprintf(stderr, "Failed to Initialize\n");
		getchar();
		glfwTerminate();
	}

//	glfwSetCursorPos(window, width/2, height/2);	// Initialize mouse position for first frame

	app.Update(0.0f, 0.0f);

	do 
	{
		double xpos = 0;
		double ypos = 0;
//		glfwGetCursorPos(window, &xpos, &ypos);
//		glfwSetCursorPos(window, width/2, height/2);	// Reset mouse position for next frame

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &xpos, &ypos);
			app.Update((float)(xpos/width), (float)(ypos/height));
		}

		double time = glfwGetTime();
		
		app.Render((float)time, (float)xpos, (float)ypos);

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

