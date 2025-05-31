#ifndef LAMP_H
#define LAMP_H

#include "../../Mesh.h"
#include "../../../Materials/Material.h"

class Lamp
{
public:
	Mesh mesh;
	glm::vec3 pos;
	glm::vec4 lightColor;

	Lamp();

	void init();

	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif