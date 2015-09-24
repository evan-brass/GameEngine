#pragma once
#include "GLRenderer.h"
#include <stdlib.h>
#include <string>

#include "glm/glm.hpp"

class Mesh;

class Object
{
public:
	// Tree
	int numChildren;
	Object **children;

	// Mesh
	Mesh * mesh;

	// loc, rot, scale
	glm::mat4 transform;

	std::string name;

	// Children
	bool addChild(Object *child);
	bool removeChild(Object *child);

	Object(std::string name, Mesh *mesh);
	virtual ~Object();
};

