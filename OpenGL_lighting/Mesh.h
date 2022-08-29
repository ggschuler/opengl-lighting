#ifndef MESH_H
#define MESH_H

#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>
#include <vector>
#include <string>
#include "Shader.h"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int ID;
	std::string type; // diffuse or specular.
};

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);
private:
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};

#endif 
