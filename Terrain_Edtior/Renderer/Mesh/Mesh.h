#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <glm/glm.hpp>

#include "../Shaders/Shader.h"
#include "../Textures/Texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoordinates;

	static std::vector<Vertex> genList(float* vertices, int numberOfVertices);
};

class Mesh
{
	unsigned int VBO, EBO;

	void setup();
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int VAO;

	std::vector<Texture> textures;

	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	
	void render(Shader shader);

	void cleanup();
};

#endif