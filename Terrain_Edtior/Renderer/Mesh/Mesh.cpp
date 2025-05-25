#include "Mesh.h"

Mesh::Mesh() {}
Mesh::Mesh
(
	std::vector<float> positions, std::vector<float> normals,
	std::vector<float> textureCoordinates, std::vector<unsigned int> indices,
	std::vector<Texture> textures
)
	: positions(positions), normals(normals), textureCoordinates(textureCoordinates),
	indices(indices), textures(textures)
{
	setup();
}

void Mesh::setup()
{
	//Create VAO, VBO, EBO 1 for each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Add EBO (indices of vertices) to VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Mesh::addAttribute(int id, int attribSize, std::vector<float> data)
{
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	VBOs.push_back(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(id);
	glVertexAttribPointer(id, attribSize, GL_FLOAT, GL_FALSE, attribSize * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void Mesh::render(Shader shader)
{
	//Set mesh's textures for shader
	for (size_t i = 0; i < textures.size(); i++)
	{
		shader.setInt(textures[i].name, i);
		textures[i].set(i);
	}

	//Draw mesh using it's VAO and unbind it at the end
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Set texture unit to default value
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);

	for (size_t i = 0; i < VBOs.size(); i++)
	{
		glDeleteBuffers(1, &VBOs[i]);
	}

	for (size_t i = 0; i < textures.size(); i++)
	{
		textures[i].cleanup();
	}
}