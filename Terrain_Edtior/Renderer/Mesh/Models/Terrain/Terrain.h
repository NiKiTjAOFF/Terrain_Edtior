#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Mesh.h"

class Terrain : public Mesh
{
	static const unsigned int VERTEX_COUNT = 128;

	//const char* GRASS_TEXTURE_PATH = "./Assets/Textures/wall.jpg";
	const char* GRASS_TEXTURE_PATH = "./Assets/Textures/grass.jpg";
	const char* GRASS_TEXTURE_NAME = "Grass";

	float x;
	float z;

public:
	static const unsigned int SIZE = 800;
	std::vector<Mesh> meshes;

	Terrain(unsigned int gridX, unsigned int gridZ);

	void init();

	void render(Shader shader);
	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif