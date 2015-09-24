#pragma once

#include <stdlib.h>
#include <string>

#include "glm/glm.hpp"

/**
* Mesh Layers: Static = no change to vertex data, Dynamic = Change to vertex data
*	Used to determine which VBO to place mesh data in.
*/
enum MeshLayer{
	MESHLAYER_STATIC,
	MESHLAYER_DYNAMIC
};

class Mesh
{
public:
	MeshLayer layer;
	std::string name;

	int vertCount;
	glm::vec3 * vertices;

	glm::vec3 * colorData;

	Mesh(std::string name);

//	bool loadMesh(std::string filename);
	bool loadMesh(const int numVertices, glm::vec3 * verts, glm::vec3 * colors);

	virtual ~Mesh();
};

