#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

class Texture
{
	static int currentId;

	const char* path;
	int width;
	int height;
	int nChannels;
public:

	int getTextureWidth();
	int getTextureHeight();

	Texture();
	Texture(const char* path, const char* name, bool defaultParams = true);

	void generate();
	void load(bool flip = true);
	void load(unsigned char* image, int width, int height, int nChannels);

	void setFilters(GLenum all);
	void setFilters(GLenum mag, GLenum min);

	void setWrap(GLenum all);
	void setWrap(GLenum s, GLenum t);

	void activate(int id);
	void bind();
	void set(int id);

	//Texture object
	int objectId;//object id essentially
	unsigned int textureId;//Texture id in GPU
	const char* name;//Shader uniform name
};

#endif