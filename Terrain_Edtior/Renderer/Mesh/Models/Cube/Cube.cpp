#include "Cube.h"

Cube::Cube(Material material)
	: material(material)
{
	init();
}

void Cube::init()
{
	int numberOfVertices = 36;
	std::vector<float> positions;
	std::vector<float> normals;
	std::vector<float> textureCoordinates;
	positions.resize(numberOfVertices * 3);
	normals.resize(numberOfVertices * 3);
	textureCoordinates.resize(numberOfVertices * 2);

	float vertices[] =
	{
		//Vertices             Normals               Textures
		//Back Face
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		//Front Face
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

		//Left Face
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		//Right Face
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		 //Bootom Face
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

		//Top Face
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
	};

	for (int i = 0; i < numberOfVertices; i++)
	{
		positions[i * 3] = vertices[i * 8];
		positions[i * 3 + 1] = vertices[i * 8 + 1];
		positions[i * 3 + 2] = vertices[i * 8 + 2];
		normals[i * 3] = vertices[i * 8 + 3];
		normals[i * 3 + 1] = vertices[i * 8 + 4];
		normals[i * 3 + 2] = vertices[i * 8 + 5];
		textureCoordinates[i * 2] = vertices[i * 8 + 6];
		textureCoordinates[i * 2 + 1] = vertices[i * 8 + 7];
	}

	std::vector<unsigned int> indices(numberOfVertices);
	for (int i = 0; i < numberOfVertices; i++)
	{
		indices[i] = i;
	}

	Texture tex0(WALL_TEXTURE_PATH, WALL_TEXTURE_NAME);
	tex0.load();
	Texture tex1(SMILEY_FACE_TEXTURE_PATH, SMILEY_FACE_TEXTURE_NAME);
	tex1.load();

	Mesh mesh(positions, normals, textureCoordinates, indices, { tex0, tex1 });
	mesh.addAttribute(0, 3, positions);
	mesh.addAttribute(1, 3, normals);
	mesh.addAttribute(2, 2, textureCoordinates);
	meshes.push_back(mesh);
}

void Cube::render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, size);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(theta), glm::vec3(rotation));
	model = glm::translate(model, pos);
	shader.setMat4("model", model);
	shader.setMat3("normalMatrix", glm::transpose(glm::inverse(model)));
	shader.set3Float("material.ambient", material.ambient);
	shader.set3Float("material.diffuse", material.diffuse);
	shader.set3Float("material.specular", material.specular);
	shader.setFloat("material.shininess", material.shininess);

	for (Mesh mesh : meshes)
	{
		mesh.render(shader);
	}
}

void Cube::cleanup()
{
	for (Mesh mesh : meshes)
	{
		mesh.cleanup();
	}
	meshes.clear();
}