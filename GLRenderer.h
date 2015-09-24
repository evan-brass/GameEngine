/*
 * GLRenderer.h
 *
 *  Created on: Apr 18, 2015
 *      Author: Evan
 */

#ifndef CLASSES_GLRENDERER_H_
#define CLASSES_GLRENDERER_H_

#include <GL/glew.h>
#include <gl/gl.h>
#include <windows.h>

#include "WindowManager.h"

// Utilities
#include "List.h"
typedef MeshCacheList List<MeshCache>;

#include "glm/glm.hpp"

class Mesh;
class Scene;
class Object;
class Camera;

struct MeshCache{ // This is my attempt to decrease the accesses to the actual Mesh
	Mesh * mesh;
	GLuint * VBOs[2];
	int firstVert;
};

class GLRenderer {
private:
	// Scene graph
	Scene *scene;

	// Context creation
	HDC deviceContext;
	HGLRC renderContext;
	List<MeshCache> meshes;

	// Static vertex array buffer
	GLuint arrayBufferStatic;
	GLuint arrayBufferStaticColor;
	int numStaticBuffer;
	glm::vec3 * staticBufferContent;
	glm::vec3 * staticBufferContentColor;
	bool staticChanged;

	// Dynamic vertex array buffer
	GLuint arrayBufferDynamic;
	GLuint arrayBufferDynamicColor;
	int numDynamicBuffer;
	glm::vec3 * dynamicBufferContent;
	glm::vec3 * dynamicBufferContentColor;
	bool dynamicChanged;

	GLuint vertArrayId; // ???
	
	// Shaders
	GLint shaderProgram;

	// Matrices
	GLuint modelViewProjectionMatrixId;

	// Drawing
	Camera * activeCamera;
	bool drawChildren(glm::mat4 *parentLocation, Object * object, Camera * camera);
	bool updateBuffers();
	bool drawBuffers();
	bool drawMesh(Mesh * mesh, glm::mat4 * transform, Camera * camera);
public:
	// Mesh functions
	bool loadMesh(Mesh * mesh);
	bool unloadMesh(Mesh * mesh);
	bool isMeshLoaded(Mesh * mesh);

	GLRenderer(Scene * scene);

	virtual ~GLRenderer();

	// Initialization
	bool initialize( HDC dummyContext, HDC realDeviceContext );

	// context creation
	HDC getDC();
	HGLRC getRC();
	bool glew();

	// Window Functions
	bool resize( int width, int height );

	// Drawing
	bool drawScene();

	// Mesh handling
	bool loadMesh(const Mesh * meshPtr);
	bool meshIsLoaded(const Mesh * meshPtr);

	// Access functions
	bool setCamera(Camera * camera);

protected:
	// Unsorted functions
	void setupScene();
	bool createBuffers();
	bool setupShaders();
};

#endif /* CLASSES_GLRENDERER_H_ */
