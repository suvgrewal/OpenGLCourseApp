#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600; // GLint type is typedef for OpenGL

GLuint VAO, VBO, shader; // Vertex Array Object, Vertex Buffer Object

void CreateTriangle()
{
	GLfloat vertices[] = {
		0.0f,  0.5f, 0.0f, // Vertex 1 (X, Y)
		0.5f, -0.5f, 0.0f, // Vertex 2 (X, Y)
	   -0.5f, -0.5f, 0.0f  // Vertex 3 (X, Y)
	};
	glGenVertexArrays(1, &VAO); // Generate 1 vertex array object
	glBindVertexArray(VAO);     // Bind the vertex array object
	glGenBuffers(1, &VBO);                      // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);         // Bind the vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy the vertices to the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); // Set the vertex attribute pointer
	glEnableVertexAttribArray(0); // Enable the vertex attribute array
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
	glBindVertexArray(0);             // Unbind the VAO
}

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