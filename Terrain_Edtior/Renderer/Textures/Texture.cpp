#include "Texture.h"

#include <iostream>

int Texture::currentId = 0;

int Texture::getTextureWidth()
{
	return width;
}
int Texture::getTextureHeight()
{
	return height;
}

Texture::Texture() {}

Texture::Texture(const char* path, const char* name, bool defaultParams)
	: path(path), name(name), objectId(currentId++)
{
	generate();

	if (defaultParams)
	{
		setFilters(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		setWrap(GL_REPEAT);
	}
}

void Texture::generate()
{
	//Create texture id, bind texture type to it
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void Texture::load(bool flip)
{
	//Image loading
	stbi_set_flip_vertically_on_load(flip);

	unsigned char* data = stbi_load(path, &width, &height, &nChannels, 0);

	GLenum colorMode = GL_RGB;
	if (nChannels == 1)
	{
		//Only red image
		colorMode = GL_RED;
	}
	else if (nChannels == 4)
	{
		//RGB + alpha channel (png for example)
		colorMode = GL_RGBA;
	}

	if (data)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
		//The collection of one texture with different resolutions
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Image not loaded at " << path << '\n';
	}

	stbi_image_free(data);
}

void Texture::load(unsigned char* image, int width, int height, int nChannels)
{
	this->path = "";
	this->width = width;
	this->height = height;
	this->nChannels = nChannels;

	GLenum colorMode = GL_RGB;
	if (nChannels == 1)
	{
		//Only red image
		colorMode = GL_RED;
	}
	else if (nChannels == 4)
	{
		//RGB + alpha channel (png for example)
		colorMode = GL_RGBA;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, image);
}

void Texture::setFilters(GLenum all)
{
	setFilters(all, all);
}
void Texture::setFilters(GLenum mag, GLenum min)
{
	//MAG_FILTER stands for upscaling the texture, when object is large but texture is small
	//MIN_FILTER stands for downscaling the texture.
	//LINEAR and NEAREST are methods to up/down scale the image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
}

void Texture::setWrap(GLenum all)
{
	setWrap(all, all);
}
void Texture::setWrap(GLenum s, GLenum t)
{
	//Our texture type is 2D, so we need to specify how to process it along the x (S) and y (T) axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t);
}

void Texture::activate(int id)
{
	//Activation of needed texture unit
	glActiveTexture(GL_TEXTURE0 + id);
}
void Texture::bind()
{
	//Activation of needed texture
	glBindTexture(GL_TEXTURE_2D, textureId);
}
void Texture::set(int id)
{
	activate(id);
	bind();
}