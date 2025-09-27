#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600; // GLint type is typedef for OpenGL

int main()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization failed!\n");
		glfwTerminate(); // terminate GLFW if initialization fails
		return 1;        // return with standard error code
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor version to 4
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set profile to core
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility
	
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	// Create a windowed mode window and its OpenGL context

	if (!mainWindow) // if window not created successfully
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	int bufferWidth;
	int bufferHeight;

	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLEW to use
	glfwMakeContextCurrent(mainWindow); // Set the current context to the created window

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed!\n");
		// get rid of the window
		glfwDestroyWindow(mainWindow); // destroy window if GLEW initialization fails
		glfwTerminate();
		return 1;
	}

	glViewport(0, 0, bufferWidth, bufferHeight); // Set the viewport size (0,0) to the size of the window
	
	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input events
		glfwPollEvents(); // Poll events from user & process them

		// Clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set clear color to red
		glClear(GL_COLOR_BUFFER_BIT);                  // clear bits

		glfwSwapBuffers(mainWindow); // Swap the front and back buffers
	}

	// Cleanup
	return 0;
}