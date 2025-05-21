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
#include "./Renderer/Textures/Texture.h"
#include "./Renderer/Model/Models/Cube.hpp"
#include "./IO/Keyboard.h"
#include "./IO/Mouse.h"
#include "./IO/Joystick.h"
#include "./IO/Camera.h"
#include "./IO/Screen.h"

void processInput(double dt);

Screen screen;

const int G_OPENGL_VERSION_MAJOR = 3;
const int G_OPENGL_VERSION_MINOR = 4;

//Shaders
const char* G_VERTEX_SHADER_PATH = "./Assets/Shaders/vertex_shader.glsl";
const char* G_FRAGMENT_SHADER_PATH = "./Assets/Shaders/fragment_shader.glsl";

float g_mixVal = 0.5f;

Camera g_camera(glm::vec3(0.0f, 0.0f, 3.0f));
float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;


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

	if (!screen.init())
	{
		std::cout << "Error while creating window\n";
		glfwTerminate();
		return -1;
	}

	//Loads all needed functions for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();
	
	//Enable z-axis
	glEnable(GL_DEPTH_TEST);

	//Setting Up Shaders
	Shader shader(G_VERTEX_SHADER_PATH, G_FRAGMENT_SHADER_PATH);
	
	/*---------------------------------Preparing Data to Render---------------------------------*/

	Cube cube1(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.75f));
	cube1.init();

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

	//Create MVP matrices
	glm::mat4 view;
	glm::mat4 projection = glm::mat4(1.0f);

	//Run window until closed for a reason
	while (!screen.shouldClose())
	{
		//Calculate time between frames
		double currentTime = glfwGetTime();
		g_deltaTime = currentTime - g_lastFrame;
		g_lastFrame = currentTime;

		//Process user inputs
		processInput(g_deltaTime);

		//Set the flush color and flush back buffer and depth buffer
		screen.update();

		//Model - matrix for every single object to convert its vertex coordinates to world coordinates
		//View - matrix that defines how to get to point from which we see the world (camera, its position, where it looks)
		//Projection - matrix of how we see the world (FOV, aspect ratio, nearest seen point, farthest seen point)
		view = g_camera.getViewMatrix();
		projection = glm::perspective(glm::radians(g_camera.getZoom()), (float)screen.s_width / (float)screen.s_height, 0.1f, 100.0f);

		//Changing mix value between 2 textures and adding MVP matrices
		shader.activate();
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		shader.setFloat("mixVal", g_mixVal);

		cube1.render(shader);

		screen.newFrame();
	}


	/*---------------------------------End Program Correctly---------------------------------*/
	cube1.cleanup();
	glfwTerminate();
	return 0;
}

void processInput(double dt)
{
	if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT))
	{
		screen.setShouldClose(true);
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