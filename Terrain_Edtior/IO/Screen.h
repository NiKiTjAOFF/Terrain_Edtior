#ifndef SCREEN_H
#define SCREEN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Screen
{
	GLFWwindow* window;
public:
	static unsigned int s_width;
	static unsigned int s_height;
	static const char* s_windowTitle;
	static const glm::vec4 s_peachColor;

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	GLFWwindow* getWindow();

	Screen();

	bool init();

	void setParameters();

	//Main loop
	void update();
	void newFrame();

	//Window closing accessor and modifier
	bool shouldClose();
	void setShouldClose(bool shouldClose);
};

#endif