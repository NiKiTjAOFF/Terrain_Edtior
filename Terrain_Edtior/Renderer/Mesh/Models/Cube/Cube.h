#ifndef CUBE_H
#define CUBE_H

#include "../../Mesh.h"

class Cube : public Mesh
{
	const char* WALL_TEXTURE_PATH = "./Assets/Textures/wall.jpg";
	const char* WALL_TEXTURE_NAME = "Wall";

	const char* SMILEY_FACE_TEXTURE_PATH = "./Assets/Textures/smile_face.png";
	const char* SMILEY_FACE_TEXTURE_NAME = "Smiley_Face";

public:
	std::vector<Mesh> meshes;
	
	Cube();
	
	void init();

	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif