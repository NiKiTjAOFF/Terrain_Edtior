#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"

class Mesh
{
	std::vector<unsigned int> VBOs;
	unsigned int EBO;

public:
	unsigned int VAO;
	std::vector<unsigned int> indices;

	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> textureCoordinates;

	std::vector<Texture> textures;

	Mesh();
	Mesh(
		std::vector<float> positions, std::vector<float> normals,
		std::vector<float> textureCoordinates, std::vector<unsigned int> indices,
		std::vector<Texture> textures
	);

	void setup();
	void addAttribute(int id, int attribSize, std::vector<float> data);
	
	void render(Shader shader);

	void cleanup();
};

#endif