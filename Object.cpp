#include "Object.h"
#include "Mesh.h"

Object::Object(std::string name, Mesh *mesh) :
	transform(1.0)
{
	this->name = name;
	this->mesh = mesh;
	numChildren = 0;
	children = static_cast<Object**>(malloc(sizeof(Object*)));
}

bool Object::addChild(Object *child){
	numChildren++;
	children = static_cast<Object**>(realloc(children, sizeof(Object*)*numChildren));
	children[numChildren - 1] = child;
	return true;
}
bool Object::removeChild(Object * child){
	for (int i = 0; i < numChildren; i++){
		if (children[i] == child){
			if (i == (numChildren - 1)){ // Child is the last child
				numChildren--;
				children = static_cast<Object**>(realloc(children, sizeof(Object*)*numChildren));
			}
			else{ // Child is in the middle of the children
				children[i] = children[numChildren - 1]; // copy the last child into the deleted shilds place and realloc
				numChildren--;
				children = static_cast<Object**>(realloc(children, sizeof(Object*)*numChildren));
			}

			return true;
		}
	}
	return false;
}

Object::~Object()
{
}
