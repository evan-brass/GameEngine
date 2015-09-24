/*
 * GLRenderer_test.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: Evan
 */

#include "GLRenderer.h"
#include "utilities.h"

GLRenderer * renderer;

void cleanup(){
	if( renderer ){
		delete renderer;
	}
}

LRESULT CALLBACK windowProcedure( const HWND wnd, const UINT msg, const WPARAM wparam, const LPARAM lparam ){
	switch( msg ){
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		//renderer->resize();
		break;
	default:
		return DefWindowProc( wnd, msg, wparam, lparam );
	}
	return 0;
}

int main( int argv, char **argc ){

	WNDCLASSEX dummyClass;
	HWND dummyHandle;

	dummyClass.cbClsExtra = 0;
	dummyClass.cbSize = sizeof( WNDCLASSEX );
	dummyClass.cbWndExtra = 0;
	dummyClass.hCursor = LoadCursor( 0, IDC_ARROW );
	dummyClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	dummyClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	dummyClass.hInstance = GetModuleHandle( 0 );
	dummyClass.hbrBackground = (HBRUSH)0;
	dummyClass.lpfnWndProc = DefWindowProc;
	dummyClass.lpszClassName = "OpenGLWindowDummy";
	dummyClass.lpszMenuName = 0;
	dummyClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;

	if ( ! RegisterClassEx( &dummyClass ) ) {
		die( "Unable to register window class" );
	}

	if ( ! ( dummyHandle = CreateWindowEx(
			//WS_EX_APPWINDOW,
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			"OpenGLWindowDummy",
			"This is the title! I bet you wouldn't guess that, huh!",
			WS_CLIPSIBLINGS |
			WS_CLIPCHILDREN |
			WS_OVERLAPPEDWINDOW,
			//WS_POPUP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			GetModuleHandle( 0 ),
			0
 ) ) ) {
		die( "Unable to create the window." );
	}

	WNDCLASSEX realClass;
	HWND realHandle;

	realClass.cbClsExtra = 0;
	realClass.cbSize = sizeof( WNDCLASSEX );
	realClass.cbWndExtra = 0;
	realClass.hCursor = LoadCursor( 0, IDC_ARROW );
	realClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	realClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	realClass.hInstance = GetModuleHandle( 0 );
	realClass.hbrBackground = (HBRUSH)0;
	realClass.lpfnWndProc = windowProcedure;
	realClass.lpszClassName = "OpenGLWindow";
	realClass.lpszMenuName = 0;
	realClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;

	if ( ! RegisterClassEx( &realClass ) ) {
		die( "Unable to register window class" );
	}

	if ( ! ( realHandle = CreateWindowEx(
			//WS_EX_APPWINDOW,
			WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
			"OpenGLWindow",
			"This is the title! I bet you wouldn't guess that, huh!",
			WS_CLIPSIBLINGS |
			WS_CLIPCHILDREN |
			WS_OVERLAPPEDWINDOW,
			//WS_POPUP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			GetModuleHandle( 0 ),
			0
 ) ) ) {
		die( "Unable to create the window." );
	}

	renderer = new GLRenderer();
	renderer->initialize( GetDC( dummyHandle ), GetDC( realHandle ) );

	DestroyWindow( dummyHandle );
	ShowWindow( realHandle, SW_SHOW );

	RECT clientRect;
	int height;
	int width;

	if ( GetClientRect( realHandle, &clientRect ) ) {
		height = clientRect.bottom - clientRect.top;
		width = clientRect.right - clientRect.left;
	} else {
		die( "unable to get client rect" );
	}
	if ( height==0 ) {
		height=1;
	}

	renderer->resize( width, height );

	MSG msg;

	while ( true ){
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ){
			if ( msg.message == WM_QUIT ){
				break;
			} else if ( msg.message == WM_SIZE ){

			}
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		if( ! renderer->drawScene() ){
			std::cout << "Rendering failed\n";
		}
	}

	if( ! ReleaseDC( realHandle, GetDC( realHandle ) ) ){
		std::cout << "Unable to release the device context\n";
	}

	cleanup();

	return 0;
}
