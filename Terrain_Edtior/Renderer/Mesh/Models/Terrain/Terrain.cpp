#include "Terrain.h"

Terrain::Terrain() 
	:size(0.1f), textureRepeat(1.0f), maxHeight(1.0f),
	sandThreshold(0.2f), grassThreshold(0.3f), rockThreshold(0.9f)
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
	std::vector<float> positions, normals, textureCoordinates;
	positions.resize(POSITIONS_SIZE);
	normals.resize(NORMALS_SIZE);
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
			//Normals
			normals[pointer * 3] = 0.0f;//x-axis
			normals[pointer * 3 + 1] = 1.0f;//y-axis
			normals[pointer * 3 + 2] = 0.0f;//z-axis
			//Texture coordinates
			textureCoordinates[pointer * 2] = (float)x / (float)width;//x-axis
			textureCoordinates[pointer * 2 + 1] = (float) z / (float)height;//y-axis
			pointer++;
		}
	}

	//TODO: calculate terrain normals
	//Counts normals according to neighboring height values
	//pointer = 0;
	//for (int z = 0; z < height; z++)
	//{
	//	for (int x = 0; x < width; x++)
	//	{
	//		int indexL = pointer * 3 + 1 - 3;//Left value
	//		int indexR = pointer * 3 + 1 + 3;//Right value
	//		int indexD = pointer * 3 + 1 - width;//Down value. Down: maybe + width
	//		int indexU = pointer * 3 + 1 + width;//Up value. Up: maybe - width
	//		float heightL = checkBounds(indexL, NORMALS_SIZE) ? positions[indexL] : 0.0f;
	//		float heightR = checkBounds(indexR, NORMALS_SIZE) ? positions[indexR] : 0.0f;
	//		float heightD = checkBounds(indexD, NORMALS_SIZE) ? positions[indexD] : 0.0f;
	//		float heightU = checkBounds(indexU, NORMALS_SIZE) ? positions[indexU] : 0.0f;
	//		glm::vec3 normal(glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU)));
	//		//Normals
	//		normals[pointer * 3] = normal.x;//x-axis
	//		normals[pointer * 3 + 1] = normal.y;//y-axis
	//		normals[pointer * 3 + 2] = normal.z;//z-axis
	//		pointer++;
	//	}
	//}

	std::vector<unsigned int> indices;
	indices.resize(NUMBER_OF_INDICES);
	pointer = 0;
	for (int gz = 0; gz < height - 1; gz++)
	{
		for (int gx = 0; gx < width- 1; gx++)
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

	Texture sand(SAND_TEXTURE_PATH, SAND_TEXTURE_NAME);
	Texture grass(GRASS_TEXTURE_PATH, GRASS_TEXTURE_NAME);
	Texture rock(ROCK_TEXTURE_PATH, ROCK_TEXTURE_NAME);
	Texture snow(SNOW_TEXTURE_PATH, SNOW_TEXTURE_NAME);
	sand.load();
	grass.load();
	rock.load();
	snow.load();

	Mesh mesh(positions, normals, textureCoordinates, indices, { sand, grass, rock, snow });
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
	meshes.clear();
}