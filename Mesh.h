#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void CreateMesh(const GLfloat *vertices, const unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);  // create the mesh
	void RenderMesh();  // draw mesh to screen
	void ClearMesh();   // clear the mesh from graphics card memory

	~Mesh();

private:
	GLuint VAO;
	GLuint VBO;
	GLuint IBO;
	GLsizei indexCount;
};