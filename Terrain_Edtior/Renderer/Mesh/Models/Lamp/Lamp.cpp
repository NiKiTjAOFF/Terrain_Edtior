#include "Lamp.h"

Lamp::Lamp()
	:lightColor(glm::vec4(1.0f))
{
	init();
}

void Lamp::init()
{
	std::vector<float> positions =
	{
		//Back vertices
		-0.5f,  0.5f, -0.5f,//Top Left
		-0.5f, -0.5f, -0.5f,//Bottom Left
		 0.5f, -0.5f, -0.5f,//Bottom Right
		 0.5f,  0.5f, -0.5f,//Top Right
		 
		 //Front vertices
		 0.5f,  0.5f,  0.5f,//Top Left
		 0.5f, -0.5f,  0.5f,//Bottom Left
		 -0.5f, -0.5f,  0.5f,//Bottom Right
		 -0.5f,  0.5f,  0.5f//Top Right
	};

	std::vector<unsigned int> indices =
	{
		//Back Face
		3, 0, 1,
		1, 2, 3,

		//Left Face
		0, 7, 6,
		6, 1, 0,

		//Forward Face
		7, 4, 5,
		5, 6, 7,

		//Right Face
		4, 3, 2,
		2, 5, 4,

		//Up Face
		4, 7, 0,
		0, 3, 4,

		//Down Face
		2, 1, 6,
		6, 5, 2
	};

	mesh = Mesh::Mesh(positions, {}, {}, indices, {});
	mesh.addAttribute(0, 3, positions);
}

void Lamp::render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, size);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(theta), glm::vec3(rotation));
	model = glm::translate(model, pos);
	shader.setMat4("model", model);

	mesh.render(shader);
}

void Lamp::cleanup()
{
	mesh.cleanup();
}