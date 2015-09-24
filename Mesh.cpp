#include "Mesh.h"


Mesh::Mesh(std::string name)
{
	layer = MESHLAYER_STATIC;
	vertCount = 0;
	this->name = name;
	vertices = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3)));
	colorData = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3)));
}

bool Mesh::loadMesh(const int numVerts, glm::vec3 * verts, glm::vec3 * colors)
{
	vertCount = numVerts;

	vertices = static_cast<glm::vec3*>(realloc(vertices, sizeof(glm::vec3)*vertCount));
	memcpy(vertices, verts, sizeof(glm::vec3)*vertCount);

	colorData = static_cast<glm::vec3*>(realloc(colorData, sizeof(glm::vec3)*vertCount));
	memcpy(colorData, colors, sizeof(glm::vec3)*vertCount);

	return true;
}

Mesh::~Mesh()
{
	free(vertices);
}
