#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader
{
public:
	Shader();

	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	 std::string ReadFile(const char* fileLocation);

	GLuint GetModelLocation();
	GLuint GetViewLocation();
	GLuint GetProjectionLocation();
	GLuint GetEyePosition();

	GLuint GetAmbientColorLocation();
	GLuint GetAmbientIntensityLocation();
	GLuint GetDirectionLocation();
	GLuint GetDiffuseIntensityLocation();
	GLuint GetShininessLocation();
	GLuint GetSpecularIntensityLocation();

	void UseShader();
	void ClearShader();

	~Shader();

private:
	GLuint shaderID;
	GLuint uniformModel;
	GLuint uniformView;
	GLuint uniformEyePosition;

	GLuint uniformProjection;
	GLuint uniformAmbientIntensity;
	GLuint uniformAmbientColor;
	GLuint uniformDirection;
	GLuint uniformDiffuseIntensity;
	GLuint uniformShininess;
	GLuint uniformSpecularIntensity;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};