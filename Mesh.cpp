#include "Mesh.h"

Mesh::Mesh()
{
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(const GLfloat* vertices, const unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
	constexpr GLsizei NUM_BUFFERS = 1;
	constexpr GLuint POSITION_ATTRIB_INDEX = 0;
	constexpr GLuint TEXTURE_COORDINATE_INDEX = 1;
	constexpr GLint POSITION_COMPONENTS = 3;
	constexpr GLint UV_COMPONENTS = 2;

	indexCount = numOfIndices;

	glGenVertexArrays(NUM_BUFFERS, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(NUM_BUFFERS, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(NUM_BUFFERS, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	GLenum type = GL_FLOAT;
	GLboolean isNormalized = GL_FALSE;
	GLsizei stride = sizeof(vertices[0]) * 5;
	void* offset = (void*) 0;
	void* offsetTextureCoord = (void*)(sizeof(vertices[0]) * 3);

	glVertexAttribPointer(POSITION_ATTRIB_INDEX, POSITION_COMPONENTS, type, isNormalized, stride, offset);
	glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
	glVertexAttribPointer(TEXTURE_COORDINATE_INDEX, UV_COMPONENTS, type, isNormalized, stride, offsetTextureCoord);
	glEnableVertexAttribArray(TEXTURE_COORDINATE_INDEX);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	// check if object exists
	if (VAO == 0 || indexCount == 0)
	{
		return;
	}

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	GLenum mode = GL_TRIANGLES;
	GLenum type = GL_UNSIGNED_INT;
	void* indices = (void*) 0;

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indices);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	constexpr int NUM_BUFFERS_TO_DELETE = 1;

	if (IBO != 0)
	{
		// delete index buffer object from graphics card memory
		
		glDeleteBuffers(NUM_BUFFERS_TO_DELETE, &IBO);
		IBO = 0;
	}

	if (VBO != 0)
	{
		// delete vertex buffer object from graphics card memory
		glDeleteBuffers(NUM_BUFFERS_TO_DELETE, &VBO);
		VBO = 0;
	}

	if (VAO != 0)
	{
		// delete vertex array object from graphics card memory
		glDeleteVertexArrays(NUM_BUFFERS_TO_DELETE, &VAO);
		VAO = 0;
	}

	indexCount = 0;
}

Mesh::~Mesh()
{
	ClearMesh();
}