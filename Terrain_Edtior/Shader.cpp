#include "Shader.h"

const int ERROR_SIZE = 512;

unsigned int Shader::getShaderProgramId()
{
	return this->id;
}

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	//Initializing values for debug purposes
	int success;
	char infoLog[ERROR_SIZE];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
	
	//Create Shader Program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	//Check for errors while linking shaders in program
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(id, ERROR_SIZE, NULL, infoLog);
		std::cout << "Shader linked in program with error: " << infoLog << '\n';
	}

	//Deleting compiled shaders because we have a ready program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::activate()
{
	glUseProgram(id);
}

std::string Shader::loadShaderSrc(const char* filename)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";
	file.open(filename);

	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
	{
		std::cout << "Couldn't open " << filename << "\n";
	}

	file.close();

	return ret;
}

GLuint Shader::compileShader(const char* filePath, GLenum type)
{
	//Initializing values for debug purposes
	int success;
	char infoLog[ERROR_SIZE];

	//Shader compilation of needed type
	GLuint shaderTypeId = glCreateShader(type);
	std::string shaderSource = loadShaderSrc(filePath);
	const GLchar* shader = shaderSource.c_str();
	glShaderSource(shaderTypeId, 1, &shader, NULL);
	glCompileShader(shaderTypeId);

	//Check for errors while compilating Shader
	glGetShaderiv(shaderTypeId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderTypeId, ERROR_SIZE, NULL, infoLog);
		std::cout << "Shader compilated with error: " << infoLog << '\n';
	}

	return shaderTypeId;
}

void Shader::setBool(const std::string& name, bool val)
{
	glUniform1i(glGetUniformLocation((id), name.c_str()), (int) val);
}

void Shader::setInt(const std::string& name, int val)
{
	glUniform1i(glGetUniformLocation((id), name.c_str()), val);
}

void Shader::setFloat(const std::string& name, float val)
{
	glUniform1f(glGetUniformLocation((id), name.c_str()), val);
}

void Shader::set4Float(const std::string& name, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation((id), name.c_str()), v1, v2, v3, v4);
}

void Shader::setMat4(const std::string& name, glm::mat4 val)
{
	glUniformMatrix4fv(glGetUniformLocation((id), name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}