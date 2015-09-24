/*
 * Scene.cpp
 *
 *  Created on: Apr 21, 2015
 *      Author: Evan
 */

#include "Scene.h"

#include "Object.h"

Scene::Scene() {
	numChildren = 0;
	children = static_cast<Object**>(malloc(sizeof(Object*)));
}

bool Scene::addChild(Object *child){
	numChildren++;
	children = static_cast<Object**>(realloc(children, sizeof(Object*)*numChildren));
	children[numChildren - 1] = child;
	return true;
}
bool Scene::removeChild(Object * child){
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

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

