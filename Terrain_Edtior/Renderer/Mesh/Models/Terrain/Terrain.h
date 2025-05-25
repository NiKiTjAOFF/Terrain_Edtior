#ifndef TERRAIN_H
#define TERRAIN_H

#include "../../Mesh.h"

enum ColorRendering
{
	TEXTURE = 0,
	COLOR,
	TEXTURE_COLOR
};

class Terrain : public Mesh
{
	const char* SAND_TEXTURE_PATH = "./Assets/Textures/sand.jpg";
	const char* SAND_TEXTURE_NAME = "sand";

	const char* GRASS_TEXTURE_PATH = "./Assets/Textures/grass.jpg";
	const char* GRASS_TEXTURE_NAME = "grass";

	const char* ROCK_TEXTURE_PATH = "./Assets/Textures/rock.jpg";
	const char* ROCK_TEXTURE_NAME = "rock";

	const char* ICE_TEXTURE_PATH = "./Assets/Textures/ice.jpg";
	const char* ICE_TEXTURE_NAME = "ice";

	bool checkBounds(int index, int size);
public:
	float size;
	float textureRepeat;
	float maxHeight;
	
	//Sand Params
	float sandHeight;
	ImVec4 sandColor;
	//Grass Params
	float grassHeight;
	ImVec4 grassColor;
	//Rock Params
	float rockHeight;
	ImVec4 rockColor;
	//Ice Params
	float iceHeight;
	ImVec4 iceColor;


	int colorType;
	int calculationMethod;
	float colorBlending;
	std::vector<Mesh> meshes;

	Terrain();

	void init(float* heightMap, int width, int height);

	void render(Shader shader);
	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif