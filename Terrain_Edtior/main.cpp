
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
const char* G_PERLIN_NOISE_FILENAME = "./Assets/Textures/Noise/perlin_noise.png";

//Terrain params
bool g_isTerrainGenerated = false;
bool g_isTerrainGenerationNeeded = false;
float g_terrainGenerationCooldown = 0.5f;
float g_lastTerrainGeneratedTime = 0.0f;

//ImGui params
bool g_isImGuiRenderNeeded = true;

//Camera params
Camera g_camera(glm::vec3(0.0f, 0.0f, 0.0f));
float g_nearPlane = 0.1f;
float g_farPlane = 1000.0f;
float g_deltaTime = 0.0f;
float g_lastFrame = 0.0f;
bool g_isCursorEnabled = false;
bool g_isWiredModeEnabled = false;

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
	PerlinNoise noise;
	Terrain terrain;
	Cube cube;

	Texture noiseTexture;
	noiseTexture.generate();
	noiseTexture.setFilters(GL_LINEAR);
	noiseTexture.setWrap(GL_REPEAT);

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

		//TODO add object placing using mousePicker
		//cube.render(cubeShader, glm::vec3(1.0f), 0.0f, glm::vec3(1.0f), MousePicker::update(projection, view));
		cube.render(cubeShader, glm::vec3(1.0f), 0.0f, glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 30.0f, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 60.0f, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
		cube.render(cubeShader, glm::vec3(1.0f), 90.0f, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(-2.0f, 0.0f, 0.0f));

		//Rendering terrain
		if (g_isTerrainGenerated)
		{
			//Vertex Shader
			terrainShader.activate();
			terrainShader.setMat4("view", view);
			terrainShader.setMat4("projection", projection);
			terrainShader.setFloat("textureRepeat", terrain.textureRepeat);
			terrainShader.setFloat("maxHeight", terrain.maxHeight);
			terrainShader.setFloat("size", terrain.size);
			//Fragment Shader
			terrainShader.setFloat("sandThreshold", terrain.sandThreshold);
			terrainShader.setFloat("grassThreshold", terrain.grassThreshold);
			terrainShader.setFloat("rockThreshold", terrain.rockThreshold);

			terrain.render(terrainShader);
		}

		//Process noise creation
		if (g_lastTerrainGeneratedTime <= g_terrainGenerationCooldown) {
			g_lastTerrainGeneratedTime += g_deltaTime;
		}
		else if (g_isTerrainGenerationNeeded)
		{
			if (!terrain.meshes.empty())
			{
				terrain.cleanup();
			}

			noise.generateNoise();
			noiseTexture.load(&noise.image[0], noise.width, noise.height, noise.nChannels);
			noise.deleteImage();
			terrain.init(&noise.heightMap[0], noise.width, noise.height);
			noise.deleteHeightMap();
			g_isTerrainGenerated = true;
			g_lastTerrainGeneratedTime = 0.0f;
		}

		//Create UI window
		if (g_isImGuiRenderNeeded) {

			//Set ImGui window width/height to fit content
			ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX));

			//Start creating ImGui window
			ImGui::Begin("Terrain Editor");

			if (ImGui::BeginTabBar("TabBar"))
			{
				//Global Settings Tab
				if (ImGui::BeginTabItem("Controls"))
				{
					ImGui::Text("Controls:                   Keyboard/Mouse     | Joystick");
					//Keyboard Controls					     
					ImGui::Text("---------------------------------------------------------");
					ImGui::Text("Keyboard controls");	     
					ImGui::Text("Exit Program:               ESC                | Circle  ");
					ImGui::Text("Move Forward:               W                  |         ");
					ImGui::Text("Move Backwards:             S                  |         ");
					ImGui::Text("Move Left:                  A                  |         ");
					ImGui::Text("Move Right:                 D                  |         ");
					ImGui::Text("Move Up:                    Space              |         ");
					ImGui::Text("Move Down:                  Left Shift         |         ");
					ImGui::Text("Close/Open ImGui Window:    Q                  |         ");
					ImGui::Text("Hide/Show Mouse Cursor:     H                  |         ");
					ImGui::Text("Enable/Disable wired mode:  M                  |         ");
					//Mouse Controls
					ImGui::Text("---------------------------------------------------------");
					ImGui::Text("Mouse Controls");
					ImGui::Text("Look Up:                    Mouse Up           |         ");
					ImGui::Text("Look Down:                  Mouse Down         |         ");
					ImGui::Text("Look Left:                  Mouse Left         |         ");
					ImGui::Text("Look Right:                 Mouse Right        |         ");
					ImGui::Text("Zoom in:                    Mouse Wheel Up     |         ");
					ImGui::Text("Zoom out:                   Mouse Wheel Down   |         ");
					ImGui::EndTabItem();
				}

				//Mouse Settings Tab
				if (ImGui::BeginTabItem("Mouse Settings"))
				{
					ImGui::SliderFloat("Mouse Sensitivity", &g_camera.mouseSensitivity, 0.1f, 1.0f);
					ImGui::SliderFloat("Mouse Wheel Sensitivity", &g_camera.wheelSensitivity, 1.0f, 10.0f);
					ImGui::SliderFloat("Movement Speed", &g_camera.speed, 1.0f, 100.0f);
					ImGui::Checkbox("Invert X-axis", &g_camera.invertX);
					ImGui::Checkbox("Invert Y-axis", &g_camera.invertY);
					ImGui::EndTabItem();
				}

				//Perlin Noise Tab
				if (ImGui::BeginTabItem("Perlin Noise"))
				{
					ImGui::SliderInt("Image width", &noise.width, 2, 512);
					ImGui::SliderInt("Image height", &noise.height, 2, 512);
					ImGui::SliderInt("Noise seed", &noise.seed, 0, 255);
					ImGui::SliderFloat("Scale", &noise.scale, 0.1f, 300.0f);
					ImGui::SliderInt("Octaves", &noise.octaves, 1, 8);
					ImGui::SliderFloat("Persistence", &noise.persistence, 0.0f, 1.0f);
					ImGui::SliderFloat("Lacunarity", &noise.lacunarity, 1.0f, 10.0f);
					ImGui::SliderFloat("Offset X", &noise.offset.x, 0.0f, 500.0f);
					ImGui::SliderFloat("Offset Y", &noise.offset.y, 0.0f, 500.0f);
					ImGui::SliderFloat("Noise Generation Cooldown", &g_terrainGenerationCooldown, 0.15f, 0.5f);
					ImGui::Checkbox("Generate Perlin Noise", &g_isTerrainGenerationNeeded);
					if (g_isTerrainGenerated)
					{
						ImGui::Image(noiseTexture.textureId, ImVec2(noiseTexture.getTextureWidth(), noiseTexture.getTextureHeight()));
					}
					ImGui::EndTabItem();
				}

				//Terrain Tab
				if (ImGui::BeginTabItem("Terrain"))
				{
					ImGui::SliderFloat("Terrain Size X", &terrain.size, 0.01f, 1.0f);
					ImGui::SliderFloat("Texture Repeat X", &terrain.textureRepeat, 1.0f, 50.0f);
					ImGui::SliderFloat("Terrain Max Height", &terrain.maxHeight, 0.1f, 100.0f);
					//ImGui::SliderFloat("Sand Threshold", &terrain.sandThreshold, 0.0f, 1.0f);
					//ImGui::SliderFloat("Grass Threshold", &terrain.grassThreshold, terrain.sandThreshold, 1.0f);
					//ImGui::SliderFloat("Rock Threshold", &terrain.rockThreshold, terrain.grassThreshold, 1.0f);
					ImGui::EndTabItem();
				}

				// End the tab bar
				ImGui::EndTabBar();
			}

			//End window creation
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
		g_isCursorEnabled = !g_isCursorEnabled;
	}

	//Switch between wired and normal mode
	if (Keyboard::keyWentDown(GLFW_KEY_M)) {
		if (!g_isWiredModeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		g_isWiredModeEnabled = !g_isWiredModeEnabled;
	}

	//Enable ImGui rendering
	if (Keyboard::keyWentDown(GLFW_KEY_Q))
	{
		g_isImGuiRenderNeeded = !g_isImGuiRenderNeeded;
	}

	//Can't move camera while ImGui opened
	if (g_isImGuiRenderNeeded)
	{
		return;
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

	if (g_isCursorEnabled)
	{
		return;
	}

	//Change Yaw and Pitch of a camera
	double dx = Mouse::getDX(), dy = Mouse::getDY();
	if (dx != 0 || dy != 0)
	{
		g_camera.updateCameraDirection(dx, dy);
	}

	//Change FOV of a camera
	double scrollDY = Mouse::getScrollDY();
	if (scrollDY != 0)
	{
		g_camera.updateCameraZoom(scrollDY);
	}
}