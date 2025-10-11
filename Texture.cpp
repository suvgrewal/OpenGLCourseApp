#include "Texture.h"

Texture::Texture() : textureID(0), width(0), height(0), bitDepth(0), fileLocation(nullptr)
{
}

Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
}

void Texture::LoadTexture()
{
	unsigned char* texData = stbi_load(fileLocation, &width, &height, &bitDepth, 0);

	if (!texData)
	{
		printf("Failed to find: %s\n", fileLocation);
	}

	// Generate & bind textures
	const GLint textureIndex = 0;
	
	const GLint numTextures = 1;
	const GLenum textureType = GL_TEXTURE_2D;
	const GLint mipMapLevel = 0;
	const GLenum internalFormat = GL_RGBA;
	const GLint border = 0;
	const GLenum format = GL_RGBA;
	const GLenum type = GL_UNSIGNED_BYTE;

	glGenTextures(numTextures, &textureID);
	glBindTexture(textureType, textureID);
	
	GLint edgeHandling = GL_MIRRORED_REPEAT;
	GLenum filterType = GL_NEAREST;

	// s and t axis wrap behavior
	glTexParameteri(textureType, GL_TEXTURE_WRAP_S, edgeHandling);
	glTexParameteri(textureType, GL_TEXTURE_WRAP_T, edgeHandling);

	glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterType);
	glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterType);

	glTexImage2D(textureType, mipMapLevel, internalFormat, width, height, border, format, type, texData);

	glGenerateMipmap(textureType);

	glBindTexture(textureType, textureIndex);

	stbi_image_free(texData);
}

void Texture::UseTexture()
{
	GLenum textureUnit = GL_TEXTURE0;
	GLenum textureType = GL_TEXTURE_2D;

	glActiveTexture(textureUnit); // sampler accesses texture through texture unit
	glBindTexture(textureType, textureID);
}

void Texture::ClearTexture()
{
	const GLint numTextures = 1;

	glDeleteTextures(numTextures, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = nullptr;
}

Texture::~Texture()
{

}
