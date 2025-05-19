#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

const int WIDTH = 640, HEIGHT = 480;
const int OPENGL_VERSION_MAJOR = 3, OPENGL_VERSION_MINOR = 4;
const int ERROR_SIZE = 512;
glm::vec4 peachColor = { 1.0f, 0.898f, 0.706f, 1.0f };//glm test

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);

int main()
{
	int success;
	char infoLog[ERROR_SIZE];

	/*---------------------------------Initialization---------------------------------*/
	//Initializes OpenGL
	glfwInit();

	//Shows OpenGL the version used and needed functions
	glfwInitHint(GLFW_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwInitHint(GLFW_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation and check
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Terrain Editor", NULL, NULL);
	if (!window)
	{
		std::cout << "Error while creating window\n";
		glfwTerminate();
		return -1;
	}

	//Use newly created window to draw frames in it
	glfwMakeContextCurrent(window);
	
	//Loads all needed functions for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Creates render area in a window, sets callback on resize
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);


	/*---------------------------------Setting Up Shaders---------------------------------*/
	
	//Vertex Shader Compilation
	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = loadShaderSrc("./vertex_shader.glsl");
	const GLchar* vertexShader = vertexShaderSource.c_str();
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderId);

	//Check for errors while compilating Vertex Shader
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, ERROR_SIZE, NULL, infoLog);
		std::cout << "Vertex Shader compilated with error: " << infoLog << '\n';
		return -1;
	}

	//Fragment Shader Compilation
	unsigned int fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSource = loadShaderSrc("./fragment_shader.glsl").c_str();
	const GLchar* fragmentShader = fragmentShaderSource.c_str();
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	//Check for errors while compilating Fragment Shader
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, ERROR_SIZE, NULL, infoLog);
		std::cout << "Fragment Shader compilated with error: " << infoLog << '\n';
		return -1;
	}

	//Create Shader Program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShaderId);
	glAttachShader(shaderProgram, fragmentShaderId);
	glLinkProgram(shaderProgram);

	//Check for errors while linking shaders in program
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, ERROR_SIZE, NULL, infoLog);
		std::cout << "Shader linked in program with error: " << infoLog << '\n';
		return -1;
	}

	//Deleting compilated shader because we have a program
	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);


	/*---------------------------------Preparing Data to Render---------------------------------*/

	//Vertex array
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.05f
	};

	//VAO, VBO creation
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind VAO, VBO, to set them as active, and provide vertices to VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Add VBO (pure vertices) as the first attribute to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	/*---------------------------------Render, Calculate, Process, Repeat---------------------------------*/

	//Run window until closed for a reason
	while (!glfwWindowShouldClose(window))
	{
		//Process user inputs
		processInput(window);

		//Set the flush color and flush back buffer
		glClearColor(peachColor.x, peachColor.y, peachColor.z, peachColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw Shapes from VAO with needed attributes, using needed program, starting from 0, 3 numbers per vertex
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Set current frame from back buffer and process any events related to a window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/*---------------------------------End Program Correctly---------------------------------*/
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSrc(const char * filename)
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