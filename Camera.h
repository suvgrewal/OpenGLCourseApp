#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat movementSpeed, GLfloat turnSpeed);
	
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition() const { return position;  }

	const glm::vec3 getFront() const { return front; }
	const glm::vec3 getRight() const { return right; }
	const glm::vec3 getUp() const { return up; }

	GLfloat getMovementSpeed() { return movementSpeed; }
	GLfloat getTurnSpeed() { return turnSpeed; }

	void addPosition(const glm::vec3& offset) { position += offset; }

	glm::mat4 calculateViewMatrix();

	~Camera();

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat yaw;
	GLfloat pitch;

	GLfloat movementSpeed;
	GLfloat turnSpeed;

	void update();
};

