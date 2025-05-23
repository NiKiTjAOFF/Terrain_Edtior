#include "Terrain.h"

Terrain::Terrain(unsigned int gridX, unsigned int gridZ)
{
	x = gridX * SIZE;
	z = gridZ * SIZE;
	init();
}

void Terrain::init()
{
	const int NUMBER_OF_VERTICES = VERTEX_COUNT * VERTEX_COUNT;
	const int NUMBER_OF_INDICES = 6 * (VERTEX_COUNT - 1) * (VERTEX_COUNT - 1);
	std::vector<float> positions, normals, textureCoordinates;
	positions.resize(NUMBER_OF_VERTICES * 3);
	normals.resize(NUMBER_OF_VERTICES * 3);
	textureCoordinates.resize(NUMBER_OF_VERTICES * 2);
	
	int vertexPointer = 0;
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			//Vertices
			positions[vertexPointer * 3] = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;//x-axis
			positions[vertexPointer * 3 + 1] = 0.0f;//y-axis
			positions[vertexPointer * 3 + 2] = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;//z-axis
			//Normals
			normals[vertexPointer * 3] = 0.0f;//x-axis
			normals[vertexPointer * 3 + 1] = 1.0f;//y-axis
			normals[vertexPointer * 3 + 2] = 0.0f;//z-axis
			//Texture coordinates
			textureCoordinates[vertexPointer * 2] = (float)j / ((float)VERTEX_COUNT - 1);//x-axis
			textureCoordinates[vertexPointer * 2 + 1] = (float)i / ((float)VERTEX_COUNT - 1);//y-axis
			vertexPointer++;
		}
	}

	std::vector<unsigned int> indices;
	indices.resize(NUMBER_OF_INDICES);
	int pointer = 0;
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz * VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	Texture tex0(GRASS_TEXTURE_PATH, GRASS_TEXTURE_NAME);
	tex0.load();

	Mesh mesh(positions, normals, textureCoordinates, indices, { tex0 });
	mesh.addAttribute(0, 3, positions);
	mesh.addAttribute(1, 3, normals);
	mesh.addAttribute(2, 2, textureCoordinates);
	meshes.push_back(mesh);
}

void Terrain::render(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	for (Mesh mesh : meshes)
	{
		mesh.render(shader);
	}
}

void Terrain::render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, size);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(theta), glm::vec3(rotation));
	model = glm::translate(model, pos);
	shader.setMat4("model", model);

	for (Mesh mesh : meshes)
	{
		mesh.render(shader);
	}
}

void Terrain::cleanup()
{
	for (Mesh mesh : meshes)
	{
		mesh.cleanup();
	}
}