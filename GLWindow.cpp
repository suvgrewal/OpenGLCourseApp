#include "GLWindow.h"

GLWindow::GLWindow() : GLWindow(800, 600)
{
}

GLWindow::GLWindow(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	bufferWidth = 0;
	bufferHeight = 0;

	mainWindow = nullptr;
}

int GLWindow::Initialize()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		const char* description;
		glfwGetError(&description);
		printf("GLFW initialization failed! %s\n", description ? description : "Unknown error");
		glfwTerminate();
		return 1;
	}

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Set minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Set profile to core
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Allow forward compatibility

	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!mainWindow) // if window not created successfully
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

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

	glEnable(GL_DEPTH_TEST); // Enable depth testing

	glViewport(0, 0, bufferWidth, bufferHeight); // Set the viewport size (0,0) to the size of the window

	return 0; // return success
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}