#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Mesh.h"

class Terrain : public Mesh
{
	const char* SAND_TEXTURE_PATH = "./Assets/Textures/sand.png";
	const char* SAND_TEXTURE_NAME = "sand";

	const char* GRASS_TEXTURE_PATH = "./Assets/Textures/grass.jpg";
	const char* GRASS_TEXTURE_NAME = "grass";

	const char* ROCK_TEXTURE_PATH = "./Assets/Textures/rock.jpg";
	const char* ROCK_TEXTURE_NAME = "rock";

	const char* SNOW_TEXTURE_PATH = "./Assets/Textures/snow.jpg";
	const char* SNOW_TEXTURE_NAME = "snow";

	bool checkBounds(int index, int size);
public:
	float size;
	float textureRepeat;
	float maxHeight;
	float sandThreshold;
	float grassThreshold;
	float rockThreshold;
	float snowThreshold;
	std::vector<Mesh> meshes;

	Terrain();

	void init(float* heightMap, int width, int height);

	void render(Shader shader);
	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif