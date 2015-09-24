/*
 * Scene.h
 *
 *  Created on: Apr 21, 2015
 *      Author: Evan
 */

#ifndef CLASSES_SCENE_H_
#define CLASSES_SCENE_H_

class Object;

class Scene {
public:
	// Children
	bool addChild(Object *child);
	bool removeChild(Object *child);

	int numChildren;
	Object **children;

	Scene();
	virtual ~Scene();
};

#endif /* CLASSES_SCENE_H_ */
