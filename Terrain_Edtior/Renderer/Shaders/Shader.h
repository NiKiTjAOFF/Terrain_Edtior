#ifndef SHADER_H
#define SHADER_H

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

	//Constructors
	Shader();
	Shader(const char* vertexShaderPath, const char* FragmentShaderPath);
	
	//Generates shader program
	void generate(const char* vertexShaderPath, const char* fragmentShaderPath);

	//Activate shader
	void activate();

	//Utility functions
	std::string loadShaderSrc(const char* filePath);
	GLuint compileShader(const char* filepath, GLenum type);

	//uniform functions
	void setBool(const std::string& name, bool val);
	void setInt(const std::string& name, int val);
	void setFloat(const std::string& name, float val);
	void set4Float(const std::string& name, float v1, float v2, float v3, float v4);
	void setVec2(const std::string& name, glm::vec2 val);
	void setMat4(const std::string& name, glm::mat4 val);
};

#endif