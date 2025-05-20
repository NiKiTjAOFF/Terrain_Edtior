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

#include <stb/stb_image.h>

#include "shader.h"

const int WIDTH = 640, HEIGHT = 480;
const int OPENGL_VERSION_MAJOR = 3, OPENGL_VERSION_MINOR = 4;
const glm::vec4 peachColor = { 1.0f, 0.898f, 0.706f, 1.0f };//glm test
const char* vertexShaderPath = "./vertex_shader.glsl";
const char* fragmentShaderPath = "./fragment_shader.glsl";
const char* wallTexturePath = "./wall.jpg";
const char* smileFaceTexturePath = "./smile_face.png";

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
	Shader shader(vertexShaderPath, fragmentShaderPath);
	shader.activate();
	shader.setMat4("transform", trans);


	/*---------------------------------Preparing Data to Render---------------------------------*/

	//Vertex array
	float vertices[] =
	{
		//Positions             Colors				 Textures
		-0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.5f,	 0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,     0.5f, 1.0f, 0.75f,	 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,    1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f,     1.0f, 0.2f, 1.0f,	 1.0f, 1.0f
	};
	unsigned int indices[] =
	{
		0, 1, 2,//first triangle
		3, 1, 2//second triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	//Add VBO's colors as the 2nd attribute to VAO
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Add VBO's texture coordinates as the 3rd attribute to VAO
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//Add EBO (indices of vertices) to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Texture setup
	//Create texture id, bind texture type to it
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	//Our texture type is 2D, so we need to specify how to process it along the x (S) and y (T) axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//MAG_FILTER stands for upscaling the texture, when object is large but texture is small
	//MIN_FILTER stands for downscaling the texture.
	//LINEAR and NEAREST are methods to up/down scale the image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	//Image loading
	int width, height, nChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(wallTexturePath, &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//The collection of one texture with different resolutions
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture " << wallTexturePath << '\n';
	}
	stbi_image_free(data);

	//Second texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load(smileFaceTexturePath, &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture " << smileFaceTexturePath << '\n';
	}
	stbi_image_free(data);

	//Loading texture to a shader via uniform.
	//We don't pass texture, it's already loaded to memory, we pass texture unit,
	//that will reference to a loaded texture
	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/*---------------------------------Render, Calculate, Process, Repeat---------------------------------*/

	//Run window until closed for a reason
	while (!glfwWindowShouldClose(window))
	{
		//Process user inputs
		processInput(window);

		//Set the flush color and flush back buffer
		glClearColor(peachColor.x, peachColor.y, peachColor.z, peachColor.a);
		glClear(GL_COLOR_BUFFER_BIT);

		//Activation of needed texture using texture units and setting unit to point to desired texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

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