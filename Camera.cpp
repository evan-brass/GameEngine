#include "Camera.h"
#include "glm/gtc/matrix_transform.hpp"

Camera::Camera(std::string name) : Object(name, 0)
{
	projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); // Fix aspect ratio
}


Camera::~Camera()
{
}
