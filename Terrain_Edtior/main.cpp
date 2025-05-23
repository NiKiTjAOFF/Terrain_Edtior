#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
#include <stb/stb_perlin.h>

#include "./Renderer/Shaders/Shader.h"
#include "./Renderer/Textures/Texture.h"
#include "./Renderer/Mesh/Models/Cube/Cube.h"
#include "./Renderer/Mesh/Models/Terrain/Terrain.h"
#include "./Renderer/Mesh/Models/Terrain/Noise/PerlinNoise.h"
#include "./Renderer/Ray_Casting/MousePicker.h"
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
const char* G_CUBE_VS_PATH = "./Assets/Shaders/Cube_VS.glsl";
const char* G_CUBE_FS_PATH = "./Assets/Shaders/Cube_FS.glsl";

const char* G_TERRAIN_VS_PATH = "./Assets/Shaders/Terrain_VS.glsl";
const char* G_TERRAIN_FS_PATH = "./Assets/Shaders/Terrain_FS.glsl";

const char* G_GLSL_VERSION = "#version 330";

//PerlinNoise params
int g_perlinWidth = 512;
int g_perlinHeight = 512;
int g_perlinSeed = 0;
const int G_N_CHANNELS = 4;
float g_noiseScale = 500.0f;
int g_octaves = 4;
float g_persistence = 0.5f;
float g_lacunarity = 2.0f;
glm::vec2 g_offset = {0.0f, 0.0f};
const char* G_PERLIN_NOISE_FILENAME = "./Assets/Textures/Noise/perlin_noise.png";
float g_noiseGenerationCooldown = 0.5f;
float g_lastNoiseGeneratedTime = 0.0f;
bool g_isNoiseGenerationNeeded = false;
bool g_isNoiseGenerated = false;
PerlinNoise noise(g_perlinWidth, g_perlinHeight, g_perlinSeed, G_N_CHANNELS,  g_noiseScale, g_octaves, g_persistence, g_lacunarity, g_offset);


float g_mixVal = 0.5f;
float g_numberOfTiles = Terrain::SIZE;

bool g_isImGuiRenderNeeded = true;

Camera g_camera(glm::vec3(0.0f, 0.0f, 0.0f));
float g_nearPlane = 0.1f;
float g_farPlane = 1000.0f;
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
	Shader terrainShader(G_TERRAIN_VS_PATH, G_TERRAIN_FS_PATH);
	Shader cubeShader(G_CUBE_VS_PATH, G_CUBE_FS_PATH);
	
	/*---------------------------------Preparing Data to Render---------------------------------*/

	Terrain terrain(5, 5);
	Cube cube;

	Texture test;
	

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

	//Setting up ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(screen.getWindow(), true);
	ImGui_ImplOpenGL3_Init(G_GLSL_VERSION);

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

		//Updates frames of ImGUI UI

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Model - matrix for every single object to convert its vertex coordinates to world coordinates
		//View - matrix that defines how to get to point from which we see the world (camera, its position, where it looks)
		//Projection - matrix of how we see the world (FOV, aspect ratio, nearest seen point, farthest seen point)
		view = g_camera.getViewMatrix();
		projection = glm::perspective(glm::radians(g_camera.getZoom()), (float)screen.s_width / (float)screen.s_height, g_nearPlane, g_farPlane);

		cubeShader.activate();
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
		cubeShader.setFloat("mixVal", g_mixVal);

		//cube.render(cubeShader, glm::vec3(1.0f), 0.0f, glm::vec3(1.0f), MousePicker::update(projection, view));//TODO
		cube.render(cubeShader, glm::vec3(1.0f), 0.0f, glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 60.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 90.0f, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));

		//Rendering terrain
		terrainShader.activate();
		terrainShader.setMat4("view", view);
		terrainShader.setMat4("projection", projection);
		terrainShader.setFloat("numberOfTiles", g_numberOfTiles);

		terrain.render(terrainShader);
		//terrain.render(terrainShader, glm::vec3(1.0f), 45.0f, glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

		//Process noise creation

		if (g_lastNoiseGeneratedTime <= g_noiseGenerationCooldown) {
			g_lastNoiseGeneratedTime += g_deltaTime;
		}
		else if (g_isNoiseGenerationNeeded)
		{
			noise.width = g_perlinWidth;
			noise.height = g_perlinHeight;
			noise.seed = g_perlinSeed;
			noise.scale = g_noiseScale;
			noise.octaves = g_octaves;
			noise.persistence = g_persistence;
			noise.lacunarity = g_lacunarity;
			noise.offset = g_offset;
			noise.generateNoise();
			test.generate();
			test.setFilters(GL_LINEAR);
			test.setWrap(GL_REPEAT);
			test.load(&noise.image[0], noise.width, noise.height, noise.nChannels);
			noise.deleteImage();
			g_isNoiseGenerated = true;
			g_lastNoiseGeneratedTime = 0.0f;
		}
		
		//Create UI window
		if (g_isImGuiRenderNeeded) {
			ImGui::Begin("Perlin Noise");
			ImGui::SliderInt("Image width", &g_perlinWidth, 1, 512);
			ImGui::SliderInt("Image height", &g_perlinHeight, 1, 512);
			ImGui::SliderInt("Noise seed", &g_perlinSeed, 0, 255);
			ImGui::SliderFloat("Scale", &g_noiseScale, 0.1f, 100.0f);
			ImGui::SliderInt("Octaves", &g_octaves, 1, 8);
			ImGui::SliderFloat("Persistence", &g_persistence, 0.0f, 1.0f);
			ImGui::SliderFloat("Lacunarity", &g_lacunarity, 1.0f, 10.0f);
			ImGui::SliderFloat("Offset X", &g_offset.x, 0.0f, 500.0f);
			ImGui::SliderFloat("Offset Y", &g_offset.y, 0.0f, 500.0f);
			ImGui::Checkbox("Generate Perlin Noise", &g_isNoiseGenerationNeeded);
			if (g_isNoiseGenerated)
			{
				ImGui::Image(test.textureId, ImVec2(test.getTextureWidth(), test.getTextureHeight()));
			}
			ImGui::End();
		}

		//Render UI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		screen.newFrame();
	}


	/*---------------------------------End Program Correctly---------------------------------*/
	//ImGui cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//OpenGL cleanup
	terrain.cleanup();
	cube.cleanup();
	glfwTerminate();
	return 0;
}

void processInput(double dt)
{
	//Shutdown window
	if (Keyboard::key(GLFW_KEY_ESCAPE) || mainJ.buttonState(GLFW_JOYSTICK_BTN_RIGHT))
	{
		screen.setShouldClose(true);
	}

	//FullScreen
	if (Keyboard::keyWentDown(GLFW_KEY_F))
	{
		if (glfwGetWindowMonitor(screen.getWindow()) == NULL) {
			// Switch to fullscreen mode
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(screen.getWindow(), monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else {
			// Switch back to windowed mode
			glfwSetWindowMonitor(screen.getWindow(), NULL, 100, 100, 800, 600, 0);
		}
	}

	//Enable/Disable cursor
	if (Keyboard::keyWentDown(GLFW_KEY_H))
	{
		if (glfwGetInputMode(screen.getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			glfwSetInputMode(screen.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(screen.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		
	}

	//Enable ImGui rendering
	if (Keyboard::keyWentDown(GLFW_KEY_Q))
	{
		g_isImGuiRenderNeeded = !g_isImGuiRenderNeeded;
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
	if ((dx != 0 || dy != 0) && Mouse::button(GLFW_MOUSE_BUTTON_LEFT))
	{
		g_camera.updateCameraDirection(dx, dy);
	}

	//Change FOV of a camera
	double scrollDY = Mouse::getScrollDY();
	if(scrollDY != 0)
	{
		g_camera.updateCameraZoom(scrollDY);
	}

	//Change camera movement relative to world axes or camera axes
	if (Keyboard::keyWentDown(GLFW_KEY_Z))
	{
		g_camera.shouldMoveRelativeToWorldYAxis = !g_camera.shouldMoveRelativeToWorldYAxis;
	}
}