#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <cctype>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "XInput.lib") 

#include "GLWindow.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

constexpr int POSITION_COMPONENTS = 4;
constexpr int TRIANGLE_VERTEX_COUNT = 3;
constexpr int NUM_UV_COMPONENTS = 2;
constexpr int NUM_NORMAL_COMPONENTS = 3;
constexpr bool verbose = false;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, float* vertices, unsigned int verticeCount, 
	                    unsigned int vertexLength, unsigned int normalOffset)
{
	// obtain normal vectors
	for (size_t i = 0; i < indiceCount; i += TRIANGLE_VERTEX_COUNT)
	{
		unsigned int in0 = indices[i] * vertexLength;
		unsigned int in1 = indices[i + 1] * vertexLength;
		unsigned int in2 = indices[i + 2] * vertexLength;

		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset;
		in1 += normalOffset;
		in2 += normalOffset;

		vertices[in0]     += normal.x;
		vertices[in0 + 1] += normal.y;
		vertices[in0 + 2] += normal.z;

		vertices[in1]     += normal.x;
		vertices[in1 + 1] += normal.y;
		vertices[in1 + 2] += normal.z;

		vertices[in2]     += normal.x;
		vertices[in2 + 1] += normal.y;
		vertices[in2 + 2] += normal.z;
	}

	// per row normalized normal vectors
	for (size_t i = 0; i < verticeCount / vertexLength; i++)
	{
		unsigned int normalOffsetPerVertex = i * vertexLength + normalOffset;
		glm::vec3 vec(vertices[normalOffsetPerVertex], vertices[normalOffsetPerVertex + 1], vertices[normalOffsetPerVertex + 2]);
		vec = glm::normalize(vec);

		vertices[normalOffsetPerVertex]     = vec.x;
		vertices[normalOffsetPerVertex + 1] = vec.y;
		vertices[normalOffsetPerVertex + 2] = vec.z;
	}
}

void CreateObjects()
{
	const int indiceCount = TRIANGLE_VERTEX_COUNT * POSITION_COMPONENTS;
	const int verticeCount = POSITION_COMPONENTS * (TRIANGLE_VERTEX_COUNT + NUM_UV_COMPONENTS + NUM_NORMAL_COMPONENTS);
	
	unsigned int indices[indiceCount] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[verticeCount] = {
	//  X      Y      Z     U     V     Nx    Ny    Nz
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom left
		0.0f,  -1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, // Top right in background
		1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom right
		0.0f,  1.0f,  0.0f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f  // Top middle
	}; 
	
	const int numVerticeColumns = verticeCount / POSITION_COMPONENTS;
	const int normalOffset = TRIANGLE_VERTEX_COUNT + NUM_UV_COMPONENTS;
	calcAverageNormals(indices, indiceCount, vertices, verticeCount, numVerticeColumns, normalOffset);

	Mesh* obj1 = new Mesh();
	const int numOfVertices = sizeof(vertices) / sizeof(vertices[0]);
	const int numOfIndices = sizeof(indices) / sizeof(indices[0]);
	obj1->CreateMesh(vertices, indices, numOfVertices, numOfIndices);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, numOfVertices, numOfIndices);
	meshList.push_back(obj2);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

char getInputDeviceTypeConnected()
{
	const int controllerIndex = 0;
	const int dwFlags = 0;
	XINPUT_CAPABILITIES pCapabilities;

	// Autodetect if XInput device connected
	if (XInputGetCapabilities(controllerIndex, dwFlags, &pCapabilities) == ERROR_SUCCESS)
	{
		if (verbose)
		{
			printf("XInput Device Autodetected");
		}

		return 'x';
	}

	return 'k';
}


inline bool isButtonPressed(const XINPUT_STATE& state, WORD button)
{
	return (state.Gamepad.wButtons & button) != 0;
}

void ProcessXInput(Camera& camera, float deltaTime)
{
	XINPUT_STATE state;
	ZeroMemory(&state, sizeof(XINPUT_STATE));

	if (XInputGetState(0, &state) == ERROR_SUCCESS)
	{
		const float STICK_DEADZONE = 0.2f;
		const float TRIGGER_DEADZONE = 0.2f;

		// Left stick values normalized
		float LX = state.Gamepad.sThumbLX / 32767.0f;
		float LY = state.Gamepad.sThumbLY / 32767.0f;
		// Right stick values normalized
		float RX = state.Gamepad.sThumbRX / 32767.0f;
		float RY = state.Gamepad.sThumbRY / 32767.0f;

		// Deadzone
		if (fabs(LX) < STICK_DEADZONE) { LX = 0.0f; }
		if (fabs(LY) < STICK_DEADZONE) { LY = 0.0f; }
		if (fabs(RX) < STICK_DEADZONE) { RX = 0.0f; }
		if (fabs(RY) < STICK_DEADZONE) { RY = 0.0f; }

		// Move position (left stick)
		glm::vec3 move{ 0.0f };
		move += camera.getFront() * LY;   // Forward/back
		move += camera.getRight() * LX;   // Left/right

		// Up/down using triggers
		float triggerUp = state.Gamepad.bRightTrigger / 255.0f;
		float triggerDown = state.Gamepad.bLeftTrigger / 255.0f;

		if (triggerUp < TRIGGER_DEADZONE) { triggerUp = 0; }
		if (triggerDown < TRIGGER_DEADZONE) { triggerDown = 0; }

		if (triggerUp || triggerDown)
		{
			move += camera.getUp() * (triggerUp - triggerDown);
		}

		bool A = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A);

		bool B = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B);

		// XINPUT_GAMEPAD_A
		if (isButtonPressed(state, XINPUT_GAMEPAD_A))
		{
			if (verbose)
			{
				printf("A is being pressed");
			}
			move += camera.getUp();
		}

		if (isButtonPressed(state, XINPUT_GAMEPAD_B))
		{
			if (verbose)
			{
				printf("B is being pressed");
			}
			move -= camera.getUp();
		}
		
		// Check magnitude threshold to avoid drift
		if (glm::length(move) > 0.001f)
		{
			camera.addPosition(move * deltaTime * camera.getMovementSpeed());
		}

		// Change looking direction (right stick)
		camera.mouseControl(RX * camera.getTurnSpeed() * deltaTime, RY * camera.getTurnSpeed() * deltaTime);
	}
}

int main()
{
	// Choose input device
	char inputDevice = getInputDeviceTypeConnected();

	// Window dimensions
	const GLint WIDTH = 800;
	const GLint HEIGHT = 600;
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;
	GLfloat currentTime = 0.0f;

	float currSize = 0.4f;

	GLWindow mainWindow = GLWindow(WIDTH, HEIGHT);

	mainWindow.Initialize();

	const float fovY = glm::radians(60.0f);  // FOV in Y direction
	const GLfloat aspectRatio = (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight();// width / height
	const float zNear = 0.1f;  // Near clipping plane
	const float zFar = 100.0f; // Far clipping plane

	const float xLoc = 0.0f;
	const float yLoc = 0.0f;
	const float zLoc = -2.5f;

	CreateObjects(); // Create triangle
	CreateShaders(); // Compile and link shaders

	glm::vec3 startPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 startUp = glm::vec3(0.0f, 1.0f, 0.0f);
	GLfloat startYaw = -90.0f;
	GLfloat startPitch = 0.0f;
	GLfloat movementSpeed = 10.0f;
	GLfloat cameraSensitivity = 20.0f;
	Camera camera = Camera{ startPosition, startUp, startYaw, startPitch, movementSpeed, cameraSensitivity };

	// TODO: put in mesh or object holding mesh for proper OOP
	std::string bricksFilename = "textures/brick.png";
	std::string dirtFilename = "textures/dirt.png";
	Texture brickTexture = Texture(bricksFilename.c_str());
	Texture dirtTexture = Texture(dirtFilename.c_str());

	brickTexture.LoadTexture();
	dirtTexture.LoadTexture();

	GLfloat redChannel = 1.0f;
	GLfloat blueChannel = 1.0f;
	GLfloat greenChannel = 1.0f;
	GLfloat ambientIntensity = 0.2f;
	GLfloat xDirection =  2.0f;   // + right
	GLfloat yDirection = -1.0f;   // + up
	GLfloat zDirection = -2.0f;   // + to camera/viewer
	GLfloat diffuseIntensity = 1.0f;
	Light mainLight = Light(redChannel, greenChannel, blueChannel, ambientIntensity, 
		                    xDirection, yDirection, zDirection, diffuseIntensity);

	GLuint uniformModel = 0;
	GLuint uniformView = 0;
	GLuint uniformProjection = 0;
	GLuint uniformAmbientColor = 0;
	GLuint uniformAmbientIntensity = 0;
	GLuint uniformDirection = 0;
	GLuint uniformDiffuseIntensity = 0;

	glm::mat4 projection = glm::perspective(fovY, aspectRatio, zNear, zFar); // Create a perspective projection matrix

	const int first = 0;
	const int count = 3;

	const GLint MATRIX_COUNT = 1;
	GLboolean TO_TRANSPOSE = GL_FALSE; // Whether to transpose matrix

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		currentTime = glfwGetTime(); // SDL_GetPerformanceCounter(); in SDL
		deltaTime = currentTime - lastTime; // (now - lastTime) * 1000 / SDL_GetPerformanceFrequency(); in SDL
		lastTime = currentTime;

		// Get and handle user input events
		glfwPollEvents(); // Poll events from user & process them

		if (std::tolower(inputDevice) == 'x')
		{
			ProcessXInput(camera, deltaTime);
		}
		else
		{
			camera.keyControl(mainWindow.getKeyStates(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set clear color to red
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformAmbientColor = shaderList[0]->GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0]->GetAmbientIntensityLocation();
		uniformDiffuseIntensity = shaderList[0]->GetDiffuseIntensityLocation();
		uniformDirection = shaderList[0]->GetDirectionLocation();

		printf("" + (int)uniformDiffuseIntensity);
		printf("" + (int)uniformDirection);

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);

		glm::mat4 model{ 1.0f }; // Create identity matrix by default

		model = glm::translate(model, glm::vec3{ 0.0f, 0.0f, -2.5f });
		model = glm::scale(model, glm::vec3{ currSize, currSize, 1.0f });
		glUniformMatrix4fv(uniformModel, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniformMatrix4fv(uniformProjection, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(projection));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh(); // Render the triangle

		model = glm::mat4(1.0f);   // Reset to identity matrix
		model = glm::translate(model, glm::vec3{ 0.0f, 1.0f, -2.5f });
		model = glm::scale(model, glm::vec3{ currSize, currSize, 1.0f });
		glUniformMatrix4fv(uniformModel, MATRIX_COUNT, TO_TRANSPOSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh(); // Render the triangle

		glUseProgram(0);

		mainWindow.swapBuffers(); // Swap the front and back buffers
	}

	// Cleanup
	return 0;
}