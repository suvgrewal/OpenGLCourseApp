#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	// uniformView = 0;
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
	CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) // If file could not be opened
	{
		printf("Failed to read %s! File doesn't exist.\n", fileLocation);
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();

	return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	shaderID = glCreateProgram(); // Create shader program

	if (!shaderID)  // If shader program creation failed
	{
		printf("Shader program creation failed!\n");
		return;
	}

	AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);    // Add vertex shader
	AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);  // Add fragment shader

	const int logLength = 1024;
	GLint result = 0;               // Compilation result
	GLchar eLog[logLength] = { 0 }; // Error log buffer
	GLsizei* infoLogLength = NULL;

	glLinkProgram(shaderID); // Link the shader program; create executables on graphics card to link program together
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result); // Get link status

	if (!result)  // Linking failed
	{
		glGetProgramInfoLog(shaderID, logLength, infoLogLength, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}
	glValidateProgram(shaderID); // Validate the shader program
	glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result); // Get validation status
	if (!result)
	{
		glGetProgramInfoLog(shaderID, logLength, infoLogLength, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}
	uniformModel = glGetUniformLocation(shaderID, "model");           // Get the location of the uniform variable "model" in the shader program
	uniformProjection = glGetUniformLocation(shaderID, "projection"); // Get the location of the uniform variable "projection" in the shader program
	// uniformView = glGetUniformLocation(shaderID, "view");             // Get the location of the uniform variable "view" in the shader program
}

// Getters
GLuint Shader::GetProjectionLocation()
{
	return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
	return uniformModel;
}

/*
GLuint Shader::GetViewLocation()
{
	// return uniformView;
	return 0;
}
*/

// Shader Usage
void Shader::UseShader()
{
	glUseProgram(shaderID); // Set the current active shader program to this program
}

void Shader::ClearShader()
{
	if (shaderID != 0)
	{
		glDeleteProgram(shaderID); // Delete the shader program from graphics card memory
		shaderID = 0;
	}
		
	uniformModel = 0;
	uniformProjection = 0;
	// uniformView = 0;
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType); // Create shader object

	if (!theShader)  // If shader object creation failed
	{
		printf("Error creating shader type %d\n", shaderType);
		return;
	}

	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	GLint codeLength[1];
	codeLength[0] = (GLint) strlen(shaderCode);

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

Shader::~Shader()
{
	ClearShader();
}