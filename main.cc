#define GLEW_STATIC

// Debugging
#define DEBUG

#include <iostream>
#include <string>
#include <windows.h>

#include "GLRenderer.h"
#include "WindowManager.h"
#include "Scene.h"
#include "utilities.h"
#include "Mesh.h"
#include "Object.h"
#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"

using namespace std;

GLRenderer * renderer = 0;
WindowManager * windowManager = 0;
Scene * scene = 0;


bool keys[256] = { false };

void cleanup(){
	delete renderer;
	delete windowManager;
}

glm::mat4 FPSViewRH(glm::vec3 eye, double pitch, double yaw)
{
	// If the pitch and yaw angles are in degrees,
	// they need to be converted to radians. Here
	// I assume the values are already converted to radians.
	float cosPitch = static_cast<float>(glm::cos(pitch));
	float sinPitch = static_cast<float>(glm::sin(pitch));
	float cosYaw = static_cast<float>(glm::cos(yaw));
	float sinYaw = static_cast<float>(glm::sin(yaw));

	glm::vec3 xaxis = { cosYaw, 0, -sinYaw };
	glm::vec3 yaxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	glm::vec3 zaxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	// Create a 4x4 view matrix from the right, up, forward and eye position vectors
	glm::mat4 viewMatrix = {
		glm::vec4(xaxis.x, yaxis.x, zaxis.x, 0),
		glm::vec4(xaxis.y, yaxis.y, zaxis.y, 0),
		glm::vec4(xaxis.z, yaxis.z, zaxis.z, 0),
		glm::vec4(-glm::dot(xaxis, eye), -glm::dot(yaxis, eye), -glm::dot(zaxis, eye), 1)
	};

	return viewMatrix;
}

void fly(Object * character){
	const float speed = 0.01f;

	glm::vec3 changeLoc(0, 0, 0);

	const float lookSpeed = 0.1f;
	float viewY = 0;
	float viewX = 0;

	// Movement
	if (keys[0x57]){ // W key
		changeLoc.z += -speed; // Move forward
	}
	else if (keys[0x53]){ // S key
		changeLoc.z += speed; // Move backward
	}

	if (keys[0x41]){ // A key
		changeLoc.x += -speed; // Move left
	}
	else if (keys[0x44]){ // D key
		changeLoc.x += speed; // Move right
	}

	if (keys[0x51]){ // Q key
		changeLoc.y += speed; // Move up
	}
	else if (keys[0x45]){ // E key
		changeLoc.y += -speed; // Move down
	}

	// looking
	if (keys[VK_UP]){ // Up arrow key
		viewX += lookSpeed; // loop up
	}
	else if (keys[VK_DOWN]){ // Down arrow key
		viewX += -lookSpeed; // look down
	}
	if (keys[VK_LEFT]){ // Left arrow key
		viewY += lookSpeed; // loop left
	}
	else if (keys[VK_RIGHT]){ // right arrow key
		viewY += -lookSpeed; // look right
	}

	float pitch = glm::radians(viewX);
	float yaw = glm::radians(viewY);

	character->transform = glm::translate(character->transform, changeLoc);
	character->transform = glm::rotate(character->transform, pitch, glm::vec3(1, 0, 0));
	character->transform = glm::rotate(character->transform, yaw, glm::vec3(0, 1, 0));
//	character->transform = FPSViewRH(changeLoc, pitch, yaw);

}

int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int			nCmdShow)
{
	bool active = true;

	scene = new Scene();

	windowManager = new WindowManager();
	renderer = new GLRenderer(scene);

	windowManager->createGLDummyWindow();
	HWND dummyHandle = windowManager->getDummyHandle();

	windowManager->createGLFinalWindow();
	HWND realHandle = windowManager->getRealHandle();

	windowManager->showWindow();

	HDC dummyDeviceContext = GetDC( dummyHandle );
	HDC deviceContext = GetDC( realHandle );

	if (!renderer->initialize(dummyDeviceContext, deviceContext)){
		cleanup();
		return 1;
	}

	int width;
	int height;

	windowManager->getSize( &width, &height );

	renderer->resize( width, height );

	// Setup our scene
	const int numVerts = 36;
	glm::vec3 Vertices[numVerts];
	// Front face
	Vertices[ 0] = glm::vec3(-1.0f, -1.0f,  1.0f);
	Vertices[ 1] = glm::vec3(-1.0f,  1.0f,  1.0f);
	Vertices[ 2] = glm::vec3( 1.0f,  1.0f,  1.0f);
	Vertices[ 3] = glm::vec3( 1.0f,  1.0f,  1.0f);
	Vertices[ 4] = glm::vec3( 1.0f, -1.0f,  1.0f);
	Vertices[ 5] = glm::vec3(-1.0f, -1.0f,  1.0f);
	// Left face
	Vertices[ 6] = glm::vec3(-1.0f, -1.0f,  1.0f);
	Vertices[ 7] = glm::vec3(-1.0f,  1.0f,  1.0f);
	Vertices[ 8] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Vertices[ 9] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Vertices[10] = glm::vec3(-1.0f,  1.0f, -1.0f);
	Vertices[11] = glm::vec3(-1.0f,  1.0f,  1.0f);
	// Right face
	Vertices[12] = glm::vec3( 1.0f, -1.0f,  1.0f);
	Vertices[13] = glm::vec3( 1.0f,  1.0f,  1.0f);
	Vertices[14] = glm::vec3( 1.0f,  1.0f, -1.0f);
	Vertices[15] = glm::vec3( 1.0f,  1.0f, -1.0f);
	Vertices[16] = glm::vec3( 1.0f, -1.0f, -1.0f);
	Vertices[17] = glm::vec3( 1.0f, -1.0f,  1.0f);
	// Back face
	Vertices[18] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Vertices[19] = glm::vec3(-1.0f,  1.0f, -1.0f);
	Vertices[20] = glm::vec3( 1.0f,  1.0f, -1.0f);
	Vertices[21] = glm::vec3( 1.0f,  1.0f, -1.0f);
	Vertices[22] = glm::vec3( 1.0f, -1.0f, -1.0f);
	Vertices[23] = glm::vec3(-1.0f, -1.0f, -1.0f);
	// Bottom face
	Vertices[24] = glm::vec3(-1.0f, -1.0f,  1.0f);
	Vertices[25] = glm::vec3(-1.0f, -1.0f, -1.0f);
	Vertices[26] = glm::vec3( 1.0f, -1.0f,  1.0f);
	Vertices[27] = glm::vec3( 1.0f, -1.0f,  1.0f);
	Vertices[28] = glm::vec3( 1.0f, -1.0f, -1.0f);
	Vertices[29] = glm::vec3(-1.0f, -1.0f, -1.0f);
	// Top face
	Vertices[30] = glm::vec3(-1.0f,  1.0f,  1.0f);
	Vertices[31] = glm::vec3(-1.0f,  1.0f, -1.0f);
	Vertices[32] = glm::vec3( 1.0f,  1.0f,  1.0f);
	Vertices[33] = glm::vec3( 1.0f,  1.0f,  1.0f);
	Vertices[34] = glm::vec3( 1.0f,  1.0f, -1.0f);
	Vertices[35] = glm::vec3(-1.0f,  1.0f, -1.0f);

	// Colors 
	glm::vec3 colors[numVerts];
	// Front face
	colors[0 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	colors[1 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	colors[2 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	colors[3 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	colors[4 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	colors[5 ] = glm::vec3(0.5f, 0.5f, 1.0f);
	// Left face
	colors[6 ] = glm::vec3(1.0f, 0.0f, 0.0f);
	colors[7 ] = glm::vec3(1.0f, 0.0f, 0.0f);
	colors[8 ] = glm::vec3(1.0f, 0.0f, 0.0f);
	colors[9 ] = glm::vec3(1.0f, 0.0f, 0.0f);
	colors[10] = glm::vec3(1.0f, 0.0f, 0.0f);
	colors[11] = glm::vec3(1.0f, 0.0f, 0.0f);
	// Right face
	colors[12] = glm::vec3(0.0f, 1.0f, 0.0f);
	colors[13] = glm::vec3(0.0f, 1.0f, 0.0f);
	colors[14] = glm::vec3(0.0f, 1.0f, 0.0f);
	colors[15] = glm::vec3(0.0f, 1.0f, 0.0f);
	colors[16] = glm::vec3(0.0f, 1.0f, 0.0f);
	colors[17] = glm::vec3(0.0f, 1.0f, 0.0f);
	// Back face
	colors[18] = glm::vec3(0.0f, 0.0f, 1.0f);
	colors[19] = glm::vec3(0.0f, 0.0f, 1.0f);
	colors[20] = glm::vec3(0.0f, 0.0f, 1.0f);
	colors[21] = glm::vec3(0.0f, 0.0f, 1.0f);
	colors[22] = glm::vec3(0.0f, 0.0f, 1.0f);
	colors[23] = glm::vec3(0.0f, 0.0f, 1.0f);
	// Bottom face
	colors[24] = glm::vec3(0.0f, 0.5f, 5.0f);
	colors[25] = glm::vec3(0.0f, 0.5f, 5.0f);
	colors[26] = glm::vec3(0.0f, 0.5f, 5.0f);
	colors[27] = glm::vec3(0.0f, 0.5f, 5.0f);
	colors[28] = glm::vec3(0.0f, 0.5f, 5.0f);
	colors[29] = glm::vec3(0.0f, 0.5f, 5.0f);
	// Top face
	colors[30] = glm::vec3(7.0f, 4.0f, 2.0f);
	colors[31] = glm::vec3(7.0f, 4.0f, 2.0f);
	colors[32] = glm::vec3(7.0f, 4.0f, 2.0f);
	colors[33] = glm::vec3(7.0f, 4.0f, 2.0f);
	colors[34] = glm::vec3(7.0f, 4.0f, 2.0f);
	colors[35] = glm::vec3(7.0f, 4.0f, 2.0f);

	Mesh * cube = new Mesh("Cube");
	cube->loadMesh(numVerts, Vertices, colors);
	renderer->loadMesh(cube);

	Object * cubeO = new Object("Cube", cube);
	cubeO->mesh = cube;
	cubeO->transform = glm::translate(cubeO->transform, glm::vec3(0.0, 0.0, 0.0));

	Camera * cam = new Camera("Primary Camera");
	float aspect = static_cast<float>(width) / static_cast<float>(height);
	cam->projectionMatrix = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
	cam->transform = glm::lookAt(
		glm::vec3(0, 0, -3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	scene->addChild(cubeO);
	scene->addChild(cam);

	renderer->setCamera(cam);

	MSG msg;
	while ( true ){
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			if ( msg.message == WM_QUIT ){
				break;
			}
			else if (msg.message == WM_KEYDOWN){
				keys[msg.wParam] = true;
			}
			else if (msg.message == WM_KEYUP){
				keys[msg.wParam] = false;
			}
			else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if ( active ) {
			if ( keys[VK_ESCAPE] ) {
				break;
			} else {
				fly(cam);
				if( ! renderer->drawScene() ){
					cout << "Rendering failed";
				}
				// Bracket begin a path 
				HDC hdc = GetDC(windowManager->getRealHandle());
				RECT r;
				GetClientRect(windowManager->getRealHandle(), &r);

				PAINTSTRUCT p;
				BeginPaint(windowManager->getRealHandle(), &p);
				BeginPath(hdc);

				// Send some text out into the world 
				TCHAR text[] = "Defenestration can be hazardous";
				TextOut(hdc, r.left, r.top, text, ARRAYSIZE(text));

				// Bracket end a path 
				EndPath(hdc);
				EndPaint(windowManager->getRealHandle(), &p);
			}
		}
	}

	cleanup();

	return 0;
}
