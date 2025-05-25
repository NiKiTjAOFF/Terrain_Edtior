#ifndef LAMP_H
#define LAMP_H

#include "../../Mesh.h"
#include "../../../Materials/Material.h"

class Lamp
{
public:
	std::vector<Mesh> meshes;
	glm::vec3 lightColor;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	Material material;

	glm::vec3 pos;

	Lamp(glm::vec3 lightColor, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void init();

	void render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos);

	void cleanup();
};

#endif