#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"

// Window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLWindow mainWindow;

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

enum class Direction { LEFT, RIGHT };

Direction direction = Direction::RIGHT;
float triOffset = 0.0f;
float triMaxOffset = 0.5f;
float triIncrement = 0.005f;

const int POSITION_COMPONENTS = 4;
const int TRIANGLE_VERTEX_COUNT = 3;
float currAngle = 0.0f;
const float degreeToRotateIncrement = 0.25f;

enum class SizeDirection { INCREASING, DECREASING };

SizeDirection sizeDirection = SizeDirection::INCREASING;

float currSize = 0.4f;
float maxSize = 0.8f;
float minSize = 0.1f;
float sizeIncrement = 0.005f;
bool changeSize = false;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[TRIANGLE_VERTEX_COUNT * POSITION_COMPONENTS] = {
		-1.0f, -1.0f, 0.0f, // Bottom left
		0.0f, -1.0f, 1.0f,  // Top right in background
		1.0f, -1.0f, 0.0f,  // Bottom right
		0.0f,  1.0f, 0.0f   // Top middle
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, sizeof(vertices) / sizeof(vertices[0]), sizeof(indices) / sizeof(indices[0]));
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = GLWindow(WIDTH, HEIGHT);

	mainWindow.Initialize();

	CreateObjects(); // Create triangle
	CreateShaders(); // Compile and link shaders

	const float fovY = glm::radians(60.0f);  // FOV in Y direction
	const GLfloat aspectRatio = (GLfloat) (mainWindow.getBufferWidth() / mainWindow.getBufferHeight()); // width / height
	const float zNear = 0.1f;  // Near clipping plane
	const float zFar = 100.0f; // Far clipping plane

	const float xLoc = 0.0f;
	const float yLoc = 0.0f;
	const float zLoc = -2.5f;

	GLuint uniformModel = 0;
	GLuint uniformProjection = 0;

	glm::mat4 projection = glm::perspective(fovY, aspectRatio, zNear, zFar); // Create a perspective projection matrix

	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	const int first = 0;
	const int count = 3;

	const GLint MATRIX_COUNT = 1;
	GLboolean TO_TRANSPOSE = GL_FALSE; // Whether to transpose the matrix as the values are loaded into the uniform variable

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get and handle user input events
		glfwPollEvents(); // Poll events from user & process them

		if (direction == Direction::RIGHT)
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			// swap directions
			direction = (direction == Direction::RIGHT) ? Direction::LEFT : Direction::RIGHT;
		}

		currAngle += degreeToRotateIncrement;
		if (currAngle >= 360.0f)
		{
			currAngle -= 360.0f;
		}

		if (changeSize)
		{
			if (sizeDirection == SizeDirection::INCREASING)
			{
				currSize += sizeIncrement;
			}
			else
			{
				currSize -= sizeIncrement;
			}

			if (currSize >= maxSize || currSize <= minSize)
			{
				sizeDirection = (sizeDirection == SizeDirection::INCREASING) ? (sizeDirection = SizeDirection::DECREASING) : (SizeDirection::INCREASING);
			}
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to red
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();

		glm::mat4 model{ 1.0f }; // Create identity matrix by default

		model = glm::translate(model, glm::vec3{ xLoc + triOffset, yLoc + triOffset, zLoc + triOffset });
		// model = glm::rotate(model, glm::radians(currAngle), glm::vec3{ 1.0f, 1.0f, 1.0f });
		model = glm::scale(model, glm::vec3{ currSize, currSize, 1.0f });

		// update uniform value by setting to triOffset
		glUniformMatrix4fv(uniformModel, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh(); // Render the triangle

		model = glm::mat4(1.0f);   // Reset to identity matrix

		model = glm::translate(model, glm::vec3{ -triOffset, 1.0f, -2.5f });
		// model = glm::rotate(model, glm::radians(currAngle), glm::vec3{ 1.0f, 1.0f, 1.0f });
		model = glm::scale(model, glm::vec3{ currSize, currSize, 1.0f });

		// update uniform value by setting to triOffset
		glUniformMatrix4fv(uniformModel, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(model));

		meshList[1]->RenderMesh(); // Render the triangle

		glUseProgram(0);

		mainWindow.swapBuffers(); // Swap the front and back buffers
	}	

	// Cleanup
	return 0;
}