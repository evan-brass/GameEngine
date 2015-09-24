/*
 * utilities.cc
 *
 *  Created on: May 8, 2015
 *      Author: Evan
 */

#include "utilities.h"

extern void cleanup();

void die( std::string message ){
	// Get the error code
	DWORD errorCode = GetLastError();
	LPVOID lpMsgBuf;
	// Connect that with an error message
	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			errorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0,
			NULL
	);
	// Check if the function failed without the system assigning an error message
	// If this happens we might as well try to keep running
	if( 0 == errorCode ){
		std::cout << "Fantom error (Refuse to exit): " << message << std::endl;
		std::cout << " - Code: " << (int)errorCode << std::endl;
		std::cout << " - Description: " << (char *)lpMsgBuf;
		goto Return;
	}
	// The normal response to a die request
	else {
		std::cout << "Error: " << message << std::endl;
		std::cout << " - Code: " << (int)errorCode << std::endl;
		std::cout << " - Description: " << (char *)lpMsgBuf;
	}
	// call destructors
	cleanup();

	exit( 1 );
Return:
	return;
}
