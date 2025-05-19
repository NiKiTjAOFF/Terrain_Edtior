#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int WIDTH = 640, HEIGHT = 480;
int OPENGL_VERSION_MAJOR = 3, OPENGL_VERSION_MINOR = 4;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
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

	//Use newly created window
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

	

	//Run window until closed for a reason
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//Set the flush color and flush back buffer
		glClearColor(1.0f, 0.898f, 0.706f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set current frame from other buffer and process any events related to a window
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}

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
