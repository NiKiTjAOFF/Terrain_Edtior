#include "Screen.h"

#include "Keyboard.h"
#include "Mouse.h"

unsigned int Screen::s_width = 640;
unsigned int Screen::s_height = 480;
const char* Screen::s_windowTitle = "Terrain Editor";
const glm::vec4 Screen::s_peachColor = { 1.0f, 0.898f, 0.706f, 1.0f };

void Screen::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	//Update render area in a window every time window changes its parameters and set them as new
	glViewport(0, 0, width, height);

	s_width = width;
	s_height = height;
}

GLFWwindow* Screen::getWindow()
{
	return window;
}

Screen::Screen()
	: window(nullptr) {}

bool Screen::init()
{
	//Window creation and check
	window = glfwCreateWindow(s_width, s_height, s_windowTitle, NULL, NULL);
	if (!window)
	{
		return false;
	}

	//Use newly created window to draw frames in it
	glfwMakeContextCurrent(window);
	return true;
}

void Screen::setParameters()
{
	//Creates render area in a window
	glViewport(0, 0, s_width, s_height);

	//Callback on resize
	glfwSetFramebufferSizeCallback(window, Screen::framebufferSizeCallback);

	//Keyboard callback
	glfwSetKeyCallback(window, Keyboard::keyCallback);

	//Mouse hide and Mouse callbacks
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);
}

void Screen::update()
{
	//Set the flush color and flush back buffer and depth buffer
	glClearColor(s_peachColor.x, s_peachColor.y, s_peachColor.z, s_peachColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Screen::newFrame()
{
	//Set current frame from back buffer and process any events related to a window
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::shouldClose()
{
	//Check if window is closed or not
	return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose)
{
	//Set window should close or not
	glfwSetWindowShouldClose(window, shouldClose);
}