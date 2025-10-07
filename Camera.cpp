#include "Camera.h"

Camera::Camera() : position(glm::vec3(0.0f, 0.0f, 0.0f)), worldUp(glm::vec3(0.0f, 1.0f, 0.0f)), yaw(0.0f), pitch(0.0f), movementSpeed(1.0f), turnSpeed(100.0f), front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	update(); // Update the camera vectors based on the initial values
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat movementSpeed, GLfloat turnSpeed) :
	position(startPosition), worldUp(startUp), yaw(startYaw), pitch(startPitch), movementSpeed(movementSpeed), turnSpeed(turnSpeed), front(0.0f, 0.0f, -1.0f)
{
	update(); // Update the camera vectors based on the initial values
}

void Camera::keyControl(bool* key_states, GLfloat deltaTime)
{
	GLfloat velocity = movementSpeed * deltaTime;

	if (key_states[GLFW_KEY_W])
	{
		position += front * velocity;
	}
	if (key_states[GLFW_KEY_S])
	{
		position -= front * velocity;
	}
	if (key_states[GLFW_KEY_A])
	{
		position -= right * velocity;
	}
	if (key_states[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if (key_states[GLFW_KEY_SPACE])
	{
		position += up * velocity;
	}
	if (key_states[GLFW_KEY_LEFT_SHIFT])
	{
		position -= up * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	// limit vertical range of motion
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front); // unit vector

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() = default;