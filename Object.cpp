#include "Object.h"
#include "Mesh.h"

Object::Object(std::string name, Mesh *mesh) : children(),
	transform(1.0)
{
	this->name = name;
	this->mesh = mesh;
}

bool Object::addChild(Object *child){
	children.add(child);
	return true;
}
bool Object::removeChild(Object * child){
	children.remove(child);
	return true;
}
int Object::count(){
	return children.count();
}

Object::~Object()
{
}
