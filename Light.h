#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambIntensity,
		  GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat difIntensity);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColorLocation,
		          GLuint diffuseIntensityLocation, GLuint directionLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

