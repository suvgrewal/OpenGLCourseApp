#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600; // GLint type is typedef for OpenGL

// Vertex Array Object, Vertex Buffer Object and Shader
GLuint VAO;
GLuint VBO;
GLuint shader;

const int POSITION_COMPONENTS = 3; // x, y, z
const int TRIANGLE_VERTEX_COUNT = 3; // triangle has 3 vertices

// Vertex Shader
static const char* vShader = "                                         \n\
#version 330                                                           \n\
                                                                       \n\
layout (location = 0) in vec3 pos;                                     \n\
                                                                       \n\
void main()                                                            \n\
{                                                                      \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);                      \n\
}                                                                      \n\
";

static const char* fShader = "                                         \n\
#version 330                                                           \n\
																	   \n\
out vec4 color;                                                        \n\
																	   \n\
void main()                                                            \n\
{                                                                      \n\
	color = vec4(0.0, 0.0, 0.0, 1.0);                                  \n\
}                                                                      \n\
";

void CreateTriangle()
{
	GLfloat vertices[TRIANGLE_VERTEX_COUNT * POSITION_COMPONENTS] = {
		-1.0f, -1.0f, 0.0f, // Bottom left
		 1.0f, -1.0f, 0.0f, // Bottom right
		 0.0f,  1.0f, 0.0f  // Top middle
	};

	GLsizei num_tris = 1;
	glGenVertexArrays(num_tris, &VAO); // Generate 1 vertex array object
	glBindVertexArray(VAO);    // Bind the vertex array object

	glGenBuffers(num_tris, &VBO); // Generate 1 vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Set buffer data

	GLuint index = 0; // Vertex attribute index
	GLint vertexSize = 3; // Number of components per vertex attribute (e.g. , x, y, z for 3D position or vec3)
	GLenum type = GL_FLOAT; // Data type of each component
	GLboolean isNormalized = GL_FALSE; // Whether fixed-point data values should be normalized
	GLsizei stride = 0; // Byte offset between consecutive vertex attributes
	void* offset = (void*)0; // Offset of the first component

	glVertexAttribPointer(index, vertexSize, type, isNormalized, stride, offset); // Set vertex attribute pointer
	glEnableVertexAttribArray(index); // Enable the vertex attribute array

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO (the call to glVertexAttribPointer registered VBO as the currently bound GL_ARRAY_BUFFER)
	
	glBindVertexArray(0); // Unbind the VAO
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // Create shader object

	if (!theShader) // If shader creation failed
	{
		printf("Error creating shader type %d\n", shaderType);
		return;
	}

	const GLchar* theCode[1]; // Hold shaderCode in array of pointers
	theCode[0] = shaderCode;  // Pointer to the shader code string

	GLint codeLength[1];      // Array of lengths of the strings
	codeLength[0] = strlen(shaderCode); // Length of the shader code string
	
	glShaderSource(theShader, 1, theCode, codeLength); // Set the source code in the shader object
	glCompileShader(theShader); // Compile the shader

	const int logLength = 1024;
	GLint result = 0;               // Compilation result
	GLchar eLog[logLength] = { 0 };      // Error log buffer
	GLsizei* infoLogLength = NULL;

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result); // Get compilation status
	
	if (!result)  // Compilation failed
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), infoLogLength, eLog);
		printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
		return;
	}


	glAttachShader(theProgram, theShader); // Attach the compiled shader to the program
}
void CompileShaders()
{
	shader = glCreateProgram(); // Create shader program

	if (!shader)  // If shader program creation failed
	{
		printf("Shader program creation failed!\n");
		return;
	}

	AddShader(shader, vShader, GL_VERTEX_SHADER);    // Add vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);  // Add fragment shader

	const int logLength = 1024;
	GLint result = 0;               // Compilation result
	GLchar eLog[logLength] = { 0 }; // Error log buffer
	GLsizei* infoLogLength = NULL;

	glLinkProgram(shader); // Link the shader program; create executables on graphics card to link program together
	glGetProgramiv(shader, GL_LINK_STATUS, &result); // Get link status

	if (!result)  // Linking failed
	{
		glGetProgramInfoLog(shader, logLength, infoLogLength, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader); // Validate the shader program
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result); // Get validation status

	if (!result)
	{
		glGetProgramInfoLog(shader, logLength, infoLogLength, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
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
	
	CreateTriangle(); // Create triangle
	CompileShaders(); // Compile and link shaders

	const int first = 0;
	const int count = 3;

	// Loop until window closed
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Get and handle user input events
		glfwPollEvents(); // Poll events from user & process them

		// Clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f); // Set clear color to red
		glClear(GL_COLOR_BUFFER_BIT);     
		
		glUseProgram(shader); // clear bits
		glBindVertexArray(VAO); // Bind the VAO (the triangle)

		glDrawArrays(GL_TRIANGLES, first, count);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind the VBO
		glUseProgram(0);

		glfwSwapBuffers(mainWindow); // Swap the front and back buffers
	}

	// Cleanup
	return 0;
}