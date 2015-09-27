/*
 * WindowManager.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: Evan
 */

#include "WindowManager.h"
#include <string>
#include "utilities.h"
#include "GLRenderer.h"

using namespace std;
namespace{
	const LPCWSTR DummyClassName = L"OGLDummyCLass";
	const LPCWSTR FinalClassName = L"OGLWindowClass";
}

extern GLRenderer * renderer;

WindowManager::WindowManager() {
	dummyHandle = 0;
	windowHandle = 0;

	dummyClass.cbClsExtra = 0;
	dummyClass.cbSize = sizeof( WNDCLASSEX );
	dummyClass.cbWndExtra = 0;
	dummyClass.hCursor = LoadCursor( 0, IDC_ARROW );
	dummyClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	dummyClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	dummyClass.hInstance = GetModuleHandle( 0 );
	dummyClass.hbrBackground = static_cast<HBRUSH>( GetStockObject( WHITE_BRUSH ) );
	dummyClass.lpfnWndProc = DefWindowProc;
	dummyClass.lpszClassName = DummyClassName;
	dummyClass.lpszMenuName = 0;
	dummyClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;

	if ( ! RegisterClassEx( &dummyClass ) ) {
		die( "Unable to register dummy window class" );
	}

	windowClass.cbClsExtra = 0;
	windowClass.cbSize = sizeof( WNDCLASSEX );
	windowClass.cbWndExtra = 0;
	windowClass.hCursor = LoadCursor( 0, IDC_ARROW );
	windowClass.hIcon = LoadIcon( 0, IDI_APPLICATION );
	windowClass.hIconSm = LoadIcon( 0, IDI_APPLICATION );
	windowClass.hInstance = GetModuleHandle( 0 );
	windowClass.hbrBackground = static_cast<HBRUSH>( GetStockObject( WHITE_BRUSH ) );
	windowClass.lpfnWndProc = mainWinProc;
	windowClass.lpszClassName = FinalClassName;
	windowClass.lpszMenuName = 0;
	windowClass.style = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;

	if ( ! RegisterClassEx( &windowClass ) ) {
		die( "Unable to register the real window class" );
	}

}

bool WindowManager::createGLDummyWindow(){
	if ( ! ( dummyHandle = CreateWindowEx(
		//WS_EX_APPWINDOW,
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		DummyClassName,
		L"This is the fake window for context creation",
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
		die( "Unable to create dummy window." );
	}

	return true;
}

bool WindowManager::createGLFinalWindow(){
	if ( ! ( windowHandle = CreateWindowEx(
		//WS_EX_APPWINDOW,
		WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		FinalClassName,
		L"Real window",
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
		die( "Unable to create real window." );
	}

	return true;
}

HWND WindowManager::getDummyHandle(){
	return this->dummyHandle;
}
HWND WindowManager::getRealHandle(){
	return this->windowHandle;
}

bool WindowManager::destroyDummy(){
	if( dummyHandle ){
		DestroyWindow( dummyHandle );
	}
	return true;
}

bool WindowManager::destroyReal(){
	if( windowHandle ){
		DestroyWindow( windowHandle );
	}
	return true;
}

bool WindowManager::showWindow(){
//	ShowWindow( dummyHandle, SW_SHOW );
	ShowWindow( windowHandle, SW_SHOW );
	SetForegroundWindow( windowHandle );
	SetFocus( windowHandle );

	return true;
}

bool WindowManager::getSize( int * width, int * height ){
	RECT clientRect;

	if ( GetClientRect( windowHandle, &clientRect ) ) {
		*height = clientRect.bottom - clientRect.top;
		*width = clientRect.right - clientRect.left;
	} else {
		die( "unable to get client rect" );
	}
	if ( *height==0 ) {
		*height=1;
	}

	return true;
}

LRESULT CALLBACK WindowManager::mainWinProc( const HWND wnd, const UINT msg, const WPARAM wparam, const LPARAM lparam ){
	switch( msg ){
	case WM_CLOSE:
		PostQuitMessage( 0 );
		break;
	case WM_SIZE:
		renderer->resize( LOWORD( lparam ), HIWORD( lparam ) );
		break;
	default:
		return DefWindowProc( wnd, msg, wparam, lparam );
	}
	return 0;
}

WindowManager::~WindowManager() {

	if( dummyHandle ){ // User forgot to destroy the dummy window
		DestroyWindow( dummyHandle );
	}
	if( windowHandle ){ // User didn't destroy the window (this is fine)
		DestroyWindow( windowHandle );
	}
	UnregisterClass( DummyClassName, GetModuleHandle(0) );
	UnregisterClass( FinalClassName, GetModuleHandle(0) );
}

