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

#include "./Renderer/Shaders/Shader.h"
#include "IO/Keyboard.h"
#include "IO/Mouse.h"
#include "IO/Joystick.h"
#include "IO/Camera.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, double dt);

int g_width = 640, g_height = 480;
const int G_OPENGL_VERSION_MAJOR = 3, G_OPENGL_VERSION_MINOR = 4;
const glm::vec4 G_PEACH_COLOR = { 1.0f, 0.898f, 0.706f, 1.0f };//glm test

//Shaders
const char* G_VERTEX_SHADER_PATH = "./Assets/Shaders/vertex_shader.glsl";
const char* G_FRAGMENT_SHADER_PATH = "./Assets/Shaders/fragment_shader.glsl";

//Textures
const char* G_WALL_TEXTURE_PATH = "./Assets/Textures/wall.jpg";
const char* G_SMILEY_FACE_TEXTURE_PATH = "./Assets/Textures/smile_face.png";

float g_mixVal = 0.5f;

//Initializing transformation matrix (rotation, scaling, translation)
glm::mat4 g_trans = glm::mat4(1.0f);

Camera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));
float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;

float g_x, g_y, g_z;


Joystick mainJ(0);

int main()
{
	/*---------------------------------Initialization---------------------------------*/
	//Initializes OpenGL
	glfwInit();

	//Shows OpenGL the version used and needed functions
	glfwInitHint(GLFW_VERSION_MAJOR, G_OPENGL_VERSION_MAJOR);
	glfwInitHint(GLFW_VERSION_MINOR, G_OPENGL_VERSION_MINOR);
	glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Window creation and check
	GLFWwindow* window = glfwCreateWindow(g_width, g_height, "Terrain Editor", NULL, NULL);
	if (!window)
	{
		std::cout << "Error while creating window\n";
		glfwTerminate();
		return -1;
	}
	
	//Hide the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Use newly created window to draw frames in it
	glfwMakeContextCurrent(window);
	
	//Loads all needed functions for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Creates render area in a window
	glViewport(0, 0, g_width, g_height);
	
	//Callback on resize
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	//Keyboard callback
	glfwSetKeyCallback(window, Keyboard::keyCallback);
	//Mouse callbacks
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
	
	//Enable z axis
	glEnable(GL_DEPTH_TEST);


	//Setting Up Shaders
	Shader shader(G_VERTEX_SHADER_PATH, G_FRAGMENT_SHADER_PATH);
	
	/*---------------------------------Preparing Data to Render---------------------------------*/

	//Vertex array
	//Quad
	//float vertices[] =
	//{
	//	//Positions             Colors				 Textures
	//	-0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.5f,	 0.0f, 0.0f,
	//	-0.5f,  0.5f, 0.0f,     0.5f, 1.0f, 0.75f,	 0.0f, 1.0f,
	//	 0.5f, -0.5f, 0.0f,     0.6f, 1.0f, 0.2f,    1.0f, 0.0f,
	//	 0.5f,  0.5f, 0.0f,     1.0f, 0.2f, 1.0f,	 1.0f, 1.0f
	//};
	//unsigned int indices[] =
	//{
	//	0, 1, 2,//first triangle
	//	3, 1, 2//second triangle
	//};

	//Cube
	float vertices[] =
	{
		//Vertices            Textures
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//VAO, VBO, 1 for each
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind VAO, VBO, to set them as active, and provide vertices to VBO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Add VBO's pure vertices as the 1st attribute to VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);
	//Add VBO's colors as the 2nd attribute to VAO
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	//Add VBO's texture coordinates as the 3rd attribute to VAO
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Add EBO (indices of vertices) to VAO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	unsigned char* data = stbi_load(G_WALL_TEXTURE_PATH, &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//The collection of one texture with different resolutions
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture " << G_WALL_TEXTURE_PATH << '\n';
	}
	stbi_image_free(data);

	//Second texture
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	data = stbi_load(G_SMILEY_FACE_TEXTURE_PATH, &width, &height, &nChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "failed to load texture " << G_SMILEY_FACE_TEXTURE_PATH << '\n';
	}
	stbi_image_free(data);

	//Loading texture to a shader via uniform.
	//We don't pass texture, it's already loaded to memory, we pass texture unit,
	//that will reference to a texture id, which will point to a texture in memory
	shader.activate();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/*---------------------------------Render, Calculate, Process, Repeat---------------------------------*/
	
	//Joystick check
	mainJ.update();
	if (mainJ.isPresent())
	{
		std::cout << mainJ.getName() << " is present.\n";
	}
	else
	{
		std::cout << "Checking for joystick... It's not present.\n";
	}

	//Point to look at
	g_x = 0.0f;
	g_y = 0.0f;
	g_z = 3.0f;
	
	//Create MVP matrices
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view;
	glm::mat4 projection = glm::mat4(1.0f);

	//Run window until closed for a reason
	while (!glfwWindowShouldClose(window))
	{
		//Calculate time between frames
		double currentTime = glfwGetTime();
		g_deltaTime = currentTime - g_lastFrame;
		g_lastFrame = currentTime;

		//Process user inputs
		processInput(window, g_deltaTime);

		//Set the flush color and flush back buffer and depth buffer
		glClearColor(G_PEACH_COLOR.x, G_PEACH_COLOR.y, G_PEACH_COLOR.z, G_PEACH_COLOR.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Activation of needed texture using texture units and setting unit to point to desired texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		//Model - matrix for every single object to convert its vertex coordinates to world coordinates
		//View - matrix that defines how to get to point from which we see the world (camera, its position, where it looks)
		//Projection - matrix of how we see the world (FOV, aspect ratio, nearest seen point, farthest seen point)
		model = glm::rotate(model, g_deltaTime * glm::radians(-55.0f), glm::vec3(0.5f));
		view = g_camera.getViewMatrix();
		projection = glm::perspective(glm::radians(g_camera.zoom), (float)g_width / (float)g_height, 0.1f, 100.0f);

		//Changing mix value between 2 textures and adding MVP matrices
		shader.activate();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setFloat("mixVal", g_mixVal);
		//Draw Shapes from VAO with needed attributes, using needed program and vertex indices
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Set current frame from back buffer and process any events related to a window
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	/*---------------------------------End Program Correctly---------------------------------*/
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	g_width = width;
	g_height = height;
}

void processInput(GLFWwindow* window, double dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT))
	{
		glfwSetWindowShouldClose(window, true);
	}

	//Changes mix value between 2 textures
	if (Keyboard::keyWentDown(GLFW_KEY_UP))
	{
		g_mixVal += .05f;
		if (g_mixVal > 1)
		{
			g_mixVal = 1.0f;
		}
	}
	if (Keyboard::keyWentDown(GLFW_KEY_DOWN))
	{
		g_mixVal -= .05f;
		if (g_mixVal < 0)
		{
			g_mixVal = 0.0f;
		}
	}

	//Move camera
	if (Keyboard::key(GLFW_KEY_W))
	{
		g_camera.updateCameraPos(CameraDirection::FORWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_S))
	{
		g_camera.updateCameraPos(CameraDirection::BACKWARD, dt);
	}
	if (Keyboard::key(GLFW_KEY_D))
	{
		g_camera.updateCameraPos(CameraDirection::RIGHT, dt);
	}
	if (Keyboard::key(GLFW_KEY_A))
	{
		g_camera.updateCameraPos(CameraDirection::LEFT, dt);
	}
	if (Keyboard::key(GLFW_KEY_SPACE))
	{
		g_camera.updateCameraPos(CameraDirection::UP, dt);
	}
	if (Keyboard::key(GLFW_KEY_LEFT_SHIFT))
	{
		g_camera.updateCameraPos(CameraDirection::DOWN, dt);
	}

	//Change Yaw and Pitch of a camera
	double dx = Mouse::getDX(), dy = Mouse::getDY();
	if (dx != 0 || dy != 0)
	{
		g_camera.updateCameraDirection(dx, dy);
	}

	//Change FOV of a camera
	double scrollDY = Mouse::getScrollDY();
	if(scrollDY != 0)
	{
		g_camera.updateCameraZoom(scrollDY);
	}
}