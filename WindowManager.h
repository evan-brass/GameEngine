/*
 * WindowManager.h
 *
 *  Created on: Apr 22, 2015
 *      Author: Evan
 */

#ifndef CLASSES_WINDOWMANAGER_H_
#define CLASSES_WINDOWMANAGER_H_

#include <windows.h>

class WindowManager {
private:
	HWND dummyHandle;
	HWND windowHandle;

	WNDCLASSEX dummyClass;
	WNDCLASSEX windowClass;

public:
	WindowManager();

	bool createGLDummyWindow();
	HWND getDummyHandle();
	bool destroyDummy();

	bool createGLFinalWindow();
	HWND getRealHandle();
	bool showWindow();
	bool destroyReal();

	bool getSize( int *width, int *height);

	static LRESULT CALLBACK mainWinProc( const HWND, const UINT, const WPARAM, const LPARAM );


	virtual ~WindowManager();
};

#endif /* CLASSES_WINDOWMANAGER_H_ */
