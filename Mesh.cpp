#include "Mesh.h"


Mesh::Mesh(std::string name) : vertices(), colorData()
{
	layer = MESHLAYER_STATIC;
	this->name = name;
}

bool Mesh::loadMesh(const int numVerts, glm::vec3 * verts, glm::vec3 * colors)
{
	vertices.add_multiple(verts, numVerts);

	colorData.add_multiple(colors, numVerts);

	return true;
}

int Mesh::count(){
	return vertices.count();
}

Mesh::~Mesh()
{
	
}
