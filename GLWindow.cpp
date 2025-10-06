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

	xChange = 0.0f;
	yChange = 0.0f;

	for (size_t i = 0; i < MAX_KEYS; i++)
	{
		key_states[i] = false;
	}

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

	// Handle Key + Mouse Input
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable mouse cursor (lock cursor to window)

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

	glfwSetWindowUserPointer(mainWindow, this);

	return 0; // return success
}

void GLWindow::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys); // Set the key callback function
	glfwSetCursorPosCallback(mainWindow, handleMouse); // Set the mouse position callback function
}

GLfloat GLWindow::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f; // reset change to 0
	return theChange;
}

GLfloat GLWindow::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f; // reset change to 0
	return theChange;
}

void GLWindow::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE); // Close window if escape is pressed
	}

	if (key >= 0 && key < MAX_KEYS)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->key_states[key] = true;
			printf("Pressed: %d\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->key_states[key] = false;
			printf("Released: %d\n", key);
		}
	}


}

void GLWindow::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	GLWindow* theWindow = static_cast<GLWindow*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved) // if mouse moved for the first time
	{
		theWindow->lastX = (GLfloat) xPos;
		theWindow->lastY = (GLfloat) yPos;
		theWindow->mouseFirstMoved = false;
	}
	theWindow->xChange = (GLfloat) xPos - theWindow->lastX; // change in x
	theWindow->yChange = theWindow->lastY - (GLfloat) yPos; // change in y
	theWindow->lastX = (GLfloat) xPos; // set lastX to current x position
	theWindow->lastY = (GLfloat) yPos; // set lastY to current y position
	printf("x: %f, y: %f\n", theWindow->xChange, theWindow->yChange);
}

GLWindow::~GLWindow()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}