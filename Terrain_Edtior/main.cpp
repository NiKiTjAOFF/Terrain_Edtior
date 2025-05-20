#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

#include "shader.h"

const int WIDTH = 640, HEIGHT = 480;
const int OPENGL_VERSION_MAJOR = 3, OPENGL_VERSION_MINOR = 4;
const glm::vec4 peachColor = { 1.0f, 0.898f, 0.706f, 1.0f };//glm test

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	//Initializing transformation matrix (rotation, scaling, translation)
	glm::mat4 trans = glm::mat4(1.0f);

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
		glfwTerminate();
		return -1;
	}

	//Creates render area in a window, sets callback on resize
	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	//Setting Up Shaders
	Shader shader("./vertex_shader.glsl", "./fragment_shader.glsl");
	shader.activate();
	shader.setMat4("transform", trans);


	/*---------------------------------Preparing Data to Render---------------------------------*/

	//Vertex array
	float vertices[] =
	{
		//Positions             Colors
		 0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.5f,
		-0.5f,  0.5f, 0.0f,     0.5f, 1.0f, 0.75f,
		-0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,
		 0.5f, -0.5f, 0.0f,     1.0f, 0.2f, 1.0f
	};
	unsigned int indices[] =
	{
		0, 1, 2,//first triangle
		2, 3, 0//second triangle
	};

	//VAO, VBO, EBO creation, 1 for each
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind VAO, VBO, to set them as active, and provide vertices to VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Add VBO's pure vertices as the 1st attribute to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	//Add VBO's colors as the 2nd attribute to VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Add EBO (indices of vertices) to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	/*---------------------------------Render, Calculate, Process, Repeat---------------------------------*/

	//Run window until closed for a reason
	while (!glfwWindowShouldClose(window))
	{
		//Process user inputs
		processInput(window);

		//Set the flush color and flush back buffer
		glClearColor(peachColor.x, peachColor.y, peachColor.z, peachColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		//Changing rotation angle
		trans = glm::rotate(trans, glm::radians((float)glfwGetTime() / 100.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMat4("transform", trans);

		//Draw Shapes from VAO with needed attributes, using needed program and vertex indices
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *) 0);

		//Set current frame from back buffer and process any events related to a window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/*---------------------------------End Program Correctly---------------------------------*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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