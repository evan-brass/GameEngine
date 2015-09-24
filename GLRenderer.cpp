/*
 * GLRenderer.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: Evan
 */
#include "GLRenderer.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>
#include <fstream>
#include "utilities.h"

#include "Scene.h"
#include "Mesh.h"
#include "Object.h"
#include "Camera.h"
#include "WindowManager.h"

#include "List.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

GLRenderer::GLRenderer(Scene * scene)
{
	this->scene = scene;
	deviceContext = 0;
	renderContext = 0;
	arrayBufferStatic = 0;
	numStaticBuffer = 0;
	staticBufferContent = 0; 
	staticBufferContentColor = 0;
	staticChanged = 0;
	arrayBufferDynamic = 0;
	numDynamicBuffer = 0;
	dynamicBufferContent = 0;
	dynamicChanged = 0;
	vertArrayId = 0;
	shaderProgram = 0;
	activeCamera = 0;

	// Matrices
	modelViewProjectionMatrixId = 0;
}

bool GLRenderer::initialize( const HDC dummyDeviceContext, const HDC inputDC ){
	HGLRC tempRenderContext = 0;

	int pixelFormat;
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		16,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if ( ! dummyDeviceContext  ) {
		die( "Renderer failed to initialize because the dummy device context was invalid." );
	}

	if ( ! ( pixelFormat = ChoosePixelFormat( dummyDeviceContext, &pfd ) ) ){
		die( "No applicable pixel format " );
	}

	if ( ! SetPixelFormat( dummyDeviceContext, pixelFormat, &pfd ) ){
		die( "The pixel format wasn't accepted" );
	}

	if ( ! ( tempRenderContext = wglCreateContext( dummyDeviceContext ) ) ) {
		die( "This window is Contextually challenged (render)" );
	}

	if ( ! wglMakeCurrent( dummyDeviceContext, tempRenderContext ) ) {
		die( "I can get the context but not apply it (render)" );
	}

	// now we have a context
	this->glew();

	this->deviceContext = inputDC;

	const int pixelFormatAttribs[] =
	{
		WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0, //End
	};

	pixelFormat = 0;
	UINT numFormats;

	wglChoosePixelFormatARB(deviceContext, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);

	if( ! SetPixelFormat( deviceContext, pixelFormat, &pfd ) ){
		die( "Couldn't set the pixel format for the real window" );
	}

	int contextAttribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		0
	};

	if( ! ( renderContext = wglCreateContextAttribsARB( deviceContext, 0, contextAttribs ) ) ){
		die( "Couldn't create the real context" );
	}

	wglMakeCurrent( deviceContext, 0 );
	wglDeleteContext( tempRenderContext );

	if( ! wglMakeCurrent( deviceContext, renderContext ) ){
		die( "Unable to set the actual render context" );
	}

	int glVersion[2] = {0};
	glGetIntegerv( GL_MAJOR_VERSION, &glVersion[0] );
	glGetIntegerv( GL_MINOR_VERSION, &glVersion[1] );

	cout << "Created a gl version " << glVersion[0] << "." << glVersion[1] << " context\n";

	if( ! wglGetCurrentContext() ){
		die( "Failed to create a valid context but I don't know why" );
	}

	glGenVertexArrays( 1, &vertArrayId );
	glBindVertexArray( vertArrayId );

	setupScene();

	if (!setupShaders()){
		return false;
	}

	if( ! this->createBuffers() ){
		die( "Failed to create the buffers." );
	}

	return true;
}

bool GLRenderer::setupShaders(){
	shaderProgram = glCreateProgram();

	GLint status;

	// Vertex Shader
	std::ifstream vertexShaderFile;
	vertexShaderFile.open("D:\\EclipseWorkspace\\C++GE\\src\\shaders\\minimal.vert");
	if (!vertexShaderFile.is_open()){
		char error[120];
		strerror_s(error, sizeof(char)*120, errno);
		return false;
	}
	std::string vertShaderText;
	vertexShaderFile.seekg(0, std::ios::end);
	vertShaderText.resize(vertexShaderFile.tellg());
	vertexShaderFile.seekg(0, std::ios::beg);
	vertexShaderFile.read(&vertShaderText[0], vertShaderText.size());
	vertexShaderFile.close();
	GLint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	const GLchar* vertPointers[1];
	vertPointers[0] = vertShaderText.c_str();
	GLint vertLengths[1];
	vertLengths[0]= strlen(vertPointers[0]);
	glShaderSource( vertexShader, 1, vertPointers, vertLengths );
	glCompileShader( vertexShader );

	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &status );
	if ( !status ){
		GLchar infoLog[1024];
		glGetShaderInfoLog( vertexShader, sizeof( infoLog ), 0, infoLog );
		fprintf(stderr, "Error compiling vertex shader : '%s'\n", infoLog);
		return false;
	}

	// Fragment Shader
	std::string fragShaderText;
	std::fstream fragmentShaderFile("D:\\EclipseWorkspace\\C++GE\\src\\shaders\\minimal.frag", std::ios::in);
	if (!fragmentShaderFile.is_open()){
		char error[120];
		strerror_s(error, sizeof(char) * 120, errno);
		return false;
	}
	fragmentShaderFile.seekg(0, std::ios::end);
	fragShaderText.resize(fragmentShaderFile.tellg());
	fragmentShaderFile.seekg(0, std::ios::beg);
	fragmentShaderFile.read(&fragShaderText[0], fragShaderText.size());
	fragmentShaderFile.close();
	GLint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	const GLchar* fragPointers[1];
	fragPointers[0] = fragShaderText.c_str();
	GLint fragLengths[1];
	fragLengths[0]= strlen(fragPointers[0]);
	glShaderSource( fragmentShader, 1, fragPointers, fragLengths );
	glCompileShader( fragmentShader );

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &status );
	if ( !status ){
		GLchar infoLog[1024];
		glGetShaderInfoLog( fragmentShader, sizeof( infoLog ), 0, infoLog );
		fprintf(stderr, "Error compiling fragment shader: '%s'\n", infoLog);
		return false;
	}

	// Combine into program
	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );
	glLinkProgram( shaderProgram );

	glGetProgramiv( shaderProgram, GL_LINK_STATUS, &status );
	if ( status == 0 ) {
		GLchar errorLog[1024];
		glGetProgramInfoLog( shaderProgram, sizeof( errorLog ), 0, errorLog );
		fprintf( stderr, "Error linking shader program: '%s'\n", errorLog );
		return false;
	}

	return true;
}

bool GLRenderer::setCamera(Camera * camera){
	activeCamera = camera;
	return true;
}


bool GLRenderer::drawScene(){
	glValidateProgram( shaderProgram );
	glUseProgram( shaderProgram );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	updateBuffers();

	// Tree recursion/update
	for (int i = 0; i < scene->numChildren; i++)
	{
		if (!drawChildren(&glm::mat4(1.0), scene->children[i], activeCamera)){
			return false;
		}
	}

	SwapBuffers( deviceContext );

	return true;
}

bool GLRenderer::drawChildren(glm::mat4 *parentLocation, Object *object, Camera * camera){
	glm::mat4 location = (*parentLocation) * object->transform;

	if (object->mesh){
		if (!isMeshLoaded(object->mesh)){
			std::cout << "All Meshes must be loaded into the renderer before rendering, mesh: " << object->mesh->name << " was referenced by object: " << object->name << "without being loaded first.";
			return false;
		}
		else {
			if (!drawMesh(object->mesh, &location, camera)){
				return false;
			}
		}
	}

	// Tree recursion
	for (int i = 0; i < object->numChildren; i++)
	{
		if (!drawChildren(&location, object->children[i], camera)){
			return false;
		}
	}
	return true;
}

bool GLRenderer::drawMesh(Mesh * mesh, glm::mat4 * location, Camera * camera){
	if (!camera){
		return false;
	}
	for (int i = 0; i < meshes.count(); i++){
		if (meshes[i].mesh == mesh){

#ifdef DEBUG
			glValidateProgram(shaderProgram);
#endif
			MeshCache *curMesh = &meshes[i];

			glm::mat4 MVP = camera->projectionMatrix * camera->transform * (*location);

			glBindBuffer(GL_ARRAY_BUFFER, *(curMesh->VBOs[0]));
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glBindBuffer(GL_ARRAY_BUFFER, *(curMesh->VBOs[1]));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glUniformMatrix4fv(modelViewProjectionMatrixId, 1, GL_FALSE, &MVP[0][0]);

			glDrawArrays(GL_TRIANGLES, curMesh->firstVert, curMesh->mesh->vertCount);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			return true;
		}
	}
	return false;
}

bool GLRenderer::updateBuffers(){
	if (staticChanged){
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferStatic);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*numStaticBuffer, staticBufferContent, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferStaticColor);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*numStaticBuffer, staticBufferContentColor, GL_STATIC_DRAW);
		staticChanged = false;
	}

	if (dynamicChanged){
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferDynamic);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*numDynamicBuffer, dynamicBufferContent, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, arrayBufferDynamicColor);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*numStaticBuffer, dynamicBufferContentColor, GL_STATIC_DRAW);
		dynamicChanged = false;
	}

	return true;
}

bool GLRenderer::loadMesh(Mesh *mesh){
	MeshCache curCache;
	curCache.mesh = mesh;

	switch (mesh->layer){
	case MESHLAYER_STATIC:
		curCache.VBOs[0] = &arrayBufferStatic;
		curCache.VBOs[1] = &arrayBufferStaticColor;
		curCache.firstVert = numStaticBuffer;
		numStaticBuffer += mesh->vertCount;
		staticBufferContent = static_cast<glm::vec3*>(realloc(staticBufferContent, sizeof(glm::vec3)*numStaticBuffer));
		memcpy(&staticBufferContent[curCache.firstVert], mesh->vertices, sizeof(glm::vec3)*mesh->vertCount);
		staticBufferContentColor = static_cast<glm::vec3*>(realloc(staticBufferContentColor, sizeof(glm::vec3)*numStaticBuffer));
		memcpy(&staticBufferContentColor[curCache.firstVert], mesh->colorData, sizeof(glm::vec3)*mesh->vertCount);
		staticChanged = true;
		break;
	case MESHLAYER_DYNAMIC:
		curCache.VBOs[0] = &arrayBufferDynamic;
		curCache.VBOs[1] = &arrayBufferDynamicColor;
		curCache.firstVert = numDynamicBuffer;
		numDynamicBuffer += mesh->vertCount;
		dynamicBufferContent = static_cast<glm::vec3*>(realloc(dynamicBufferContent, sizeof(glm::vec3)*numDynamicBuffer));
		memcpy(&dynamicBufferContent[curCache.firstVert], mesh->vertices, sizeof(glm::vec3)*mesh->vertCount);
		dynamicBufferContentColor = static_cast<glm::vec3*>(realloc(dynamicBufferContentColor, sizeof(glm::vec3)*numStaticBuffer));
		memcpy(&dynamicBufferContentColor[curCache.firstVert], mesh->colorData, sizeof(glm::vec3)*mesh->vertCount);
		dynamicChanged = true;
		break;
	default:
		std::cout << "Unknown mesh layer: " << mesh->layer;
		return false;
	}
	meshes.add(curCache);
	return true;
}
bool GLRenderer::unloadMesh(Mesh * mesh){
	for (int i = 0; i < meshes.count(); i++){
		if (meshes[i].mesh == mesh){
			MeshCache curMesh = meshes[i];
			switch (meshes[i].mesh->layer){
			case MESHLAYER_STATIC:
				memcpy(&staticBufferContent[curMesh.firstVert], 
					&staticBufferContent[curMesh.firstVert + curMesh.mesh->vertCount], 
					sizeof(glm::vec3)*(numStaticBuffer -curMesh.firstVert - curMesh.mesh->vertCount));
				numStaticBuffer -= curMesh.mesh->vertCount;
				staticBufferContent = static_cast<glm::vec3*>(realloc(staticBufferContent, sizeof(glm::vec3)*numStaticBuffer));
				break;
			case MESHLAYER_DYNAMIC:
				memcpy(&dynamicBufferContent[curMesh.firstVert],
					&dynamicBufferContent[curMesh.firstVert + curMesh.mesh->vertCount],
					sizeof(glm::vec3)*(numDynamicBuffer - curMesh.firstVert - curMesh.mesh->vertCount));
				numDynamicBuffer -= curMesh.mesh->vertCount;
				dynamicBufferContent = static_cast<glm::vec3*>(realloc(dynamicBufferContent, sizeof(glm::vec3)*numDynamicBuffer));
				break;
			default:
				// failed
				return false;
			}
			meshes.remove(meshes[i]);

			return true;
		}
	}
	return false;
}

bool GLRenderer::isMeshLoaded(Mesh *mesh){
	for (int i=0; i < meshes.count(); i++){
		if (meshes[i].mesh == mesh){
			return true;
		}
	}
	return false;
}

bool GLRenderer::createBuffers(){
	// Uniform to supply the model view projection matrix to the shaders
	glGetUniformLocation(shaderProgram, "GLM");

	glGenBuffers(1, &arrayBufferStatic);
	glGenBuffers(1, &arrayBufferStaticColor);

	glGenBuffers(1, &arrayBufferDynamic);
	glGenBuffers(1, &arrayBufferDynamicColor);

	return true;
}

void GLRenderer::setupScene(){
	glClearColor ( 0.0, 0.0, 0.0, 0.0 );
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return;
}

HDC GLRenderer::getDC(){
	return this->deviceContext;
}

HGLRC GLRenderer::getRC(){
	return this->renderContext;
}

bool GLRenderer::glew(){
	glewExperimental = GL_TRUE;
	GLenum stat = glewInit();
	//GLenum stat = GLEW_OK;
	if( stat != GLEW_OK ){
		die( "Glew couldn't initialize" );
	}
	return true;
}

bool GLRenderer::resize( int width, int height ){
//	glViewport(0, 0, width-50, height-50);

	return true;
}

GLRenderer::~GLRenderer() {
	if( !wglMakeCurrent( 0, 0 ) ){
		cout << "Unable to disable rendering context";
	}
	if( !wglDeleteContext( renderContext ) ){
		cout << "Unable to delete rendering context";
	}
}

