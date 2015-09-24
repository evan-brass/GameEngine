#pragma once
#include "Object.h"
#include <string>

#include "glm/glm.hpp"

class Camera : public Object
{
public:
	// pojection matrix (the view matrix is the object's location matrix)
	glm::mat4 projectionMatrix;

	// Initialization and Destruction
	Camera(std::string name);
	virtual ~Camera();
};

