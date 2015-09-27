#pragma once
#include "GLRenderer.h"
#include <stdlib.h>
#include <string>

#include "glm/glm.hpp"

#include "List.h"

class Mesh;

class Object
{
public:
	// Tree
	List<Object*> children;

	// Mesh
	Mesh * mesh;

	// loc, rot, scale
	glm::mat4 transform;

	std::string name;

	// Children
	bool addChild(Object *child);
	bool removeChild(Object *child);
	int count();

	Object(std::string name, Mesh *mesh);
	virtual ~Object();
};

