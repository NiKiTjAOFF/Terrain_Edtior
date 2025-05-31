#include "Terrain.h"

Terrain::Terrain()
	:size(0.1f), textureRepeat(1.0f), maxHeight(1.0f),
	//Sand
	sandHeight(0.1f), sandColor(ImVec4(0.910f, 0.704f, 0.0273f, 1.0f)),
	//Grass
	grassHeight(0.2f), grassColor(ImVec4(0.341f, 0.810f, 0.316f, 1.0f)),
	//Rock
	rockHeight(0.25f), rockColor(ImVec4(0.680f, 0.503f, 0.645f, 1.0f)),
	//Ice
	iceHeight(0.9f), iceColor(ImVec4(0.164f, 0.579f, 0.820f, 1.0f)),
	isLightNeeded(true), colorType(1), calculationMethod(1)
{}

bool Terrain::checkBounds(int index, int size)
{
	return ((index >= 0) && (index < size)) ? true : false;
}

void Terrain::init(float* heightMap, int width, int height)
{
	const int NUMBER_OF_VERTICES = width * height;
	const int NUMBER_OF_INDICES = 6 * (height - 1) * (width - 1);
	const int POSITIONS_SIZE = NUMBER_OF_VERTICES * 3;
	const int NORMALS_SIZE = POSITIONS_SIZE;
	const int TEXTURE_COORDINATES = NUMBER_OF_VERTICES * 2;
	std::vector<float> positions, textureCoordinates;
	positions.resize(POSITIONS_SIZE);
	textureCoordinates.resize(TEXTURE_COORDINATES);
	
	int pointer = 0;
	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; x++)
		{
			//Vertices
			positions[pointer * 3] = x;//x-axis
			positions[pointer * 3 + 1] = heightMap[z * width + x];//y-axis
			positions[pointer * 3 + 2] = z;//z-axis
			//Texture coordinates
			textureCoordinates[pointer * 2] = (float)x / (float)(width - 1);//x-axis
			textureCoordinates[pointer * 2 + 1] = (float) z / (float)(height - 1);//y-axis
			pointer++;
		}
	}

	//Indices initialization
	std::vector<unsigned int> indices;
	indices.resize(NUMBER_OF_INDICES);
	pointer = 0;
	for (int gz = 0; gz < height - 1; gz++)
	{
		for (int gx = 0; gx < width - 1; gx++)
		{
			int topLeft = (gz * width) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * width) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	//Normal calculation via cross product of 2 vectors of one triangle
	std::vector<float> normals;
	normals.resize(NORMALS_SIZE);
	for (int i = 0; i < indices.size(); i += 3)
	{
		int index0 = indices[i];
		int index1 = indices[i + 1];
		int index2 = indices[i + 2];
		glm::vec3 pos0 = glm::vec3(positions[index0 * 3], positions[index0 * 3 + 1], positions[index0 * 3 + 2]);
		glm::vec3 pos1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1], positions[index1 * 3 + 2]);
		glm::vec3 pos2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1], positions[index2 * 3 + 2]);
		glm::vec3 vector1 = pos2 - pos1;
		glm::vec3 vector2 = pos0 - pos1;
		glm::vec3 normal = glm::cross(vector1, vector2);
		normal = glm::normalize(normal);

		normals[index0 * 3] += normal.x;
		normals[index0 * 3 + 1] += normal.y;
		normals[index0 * 3 + 2] += normal.z;
		normals[index1 * 3] += normal.x;
		normals[index1 * 3 + 1] += normal.y;
		normals[index1 * 3 + 2] += normal.z;
		normals[index2 * 3] += normal.x;
		normals[index2 * 3 + 1] += normal.y;
		normals[index2 * 3 + 2] += normal.z;
	}

	//Normalization of normals sum between 1-6 vertices
	pointer = 0;
	for (int i = 0; i < normals.size(); i++)
	{
		glm::vec3 normal = glm::vec3(normal[pointer * 3], normal[pointer * 3 + 1], normal[pointer * 3 + 2]);
		glm::vec3 normalizedNormal = glm::normalize(normal);
		normals[pointer * 3] = normalizedNormal.x;
		normals[pointer * 3 + 1] = normalizedNormal.y;
		normals[pointer * 3 + 2] = normalizedNormal.z;
	}

	Texture sand(SAND_TEXTURE_PATH, SAND_TEXTURE_NAME);
	Texture grass(GRASS_TEXTURE_PATH, GRASS_TEXTURE_NAME);
	Texture rock(ROCK_TEXTURE_PATH, ROCK_TEXTURE_NAME);
	Texture snow(ICE_TEXTURE_PATH, ICE_TEXTURE_NAME);
	sand.load();
	grass.load();
	rock.load();
	snow.load();

	mesh = Mesh::Mesh(positions, normals, textureCoordinates, indices, { sand, grass, rock, snow });
	mesh.addAttribute(0, 3, positions);
	mesh.addAttribute(1, 3, normals);
	mesh.addAttribute(2, 2, textureCoordinates);
}

void Terrain::render(Shader shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	mesh.render(shader);
}

void Terrain::render(Shader shader, glm::vec3 size, float theta, glm::vec3 rotation, glm::vec3 pos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, size);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(theta), glm::vec3(rotation));
	model = glm::translate(model, pos);
	shader.setMat4("model", model);

	mesh.render(shader);

}

void Terrain::cleanup()
{
	mesh.cleanup();
}