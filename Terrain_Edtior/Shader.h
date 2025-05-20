#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
	unsigned int id;
public:

	//Get/Set
	unsigned int getShaderProgramId();

	Shader(const char* vertexShaderPath, const char* FragmentShaderPath);
	void activate();

	//Utility functions
	std::string loadShaderSrc(const char* filePath);
	GLuint compileShader(const char* filepath, GLenum type);

	//uniform functions
	void setMat4(const std::string& name, glm::mat4 val);
};
