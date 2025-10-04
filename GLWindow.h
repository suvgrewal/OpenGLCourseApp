#pragma once

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GLWindow
{
public:
	GLWindow();
	GLWindow(GLint windowWidth, GLint windowHeight);

	int Initialize();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
	// bool* getKeys() { return keys; }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }

	~GLWindow();

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;
};