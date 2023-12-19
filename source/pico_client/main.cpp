#include "engine.h"

#include <cassert>

static const int nWindowWidth = 640;
static const int nWindowHeight = 640;

/**
 * @file
 *
 * @brief main.cpp
 *
 * this file stores the entry point of the client application
 */

/**
 * @brief client application for the "checkers" game on pico
 * @author Ilkiv Danylo KI-48
 *
 * @version v1.0.0
 * @date 12/12/24
 *
 * @warning for the application to work, it is necessary to connect the "RP PICO" microcontroller with the server application recorded on it to the COM port!
 */

//! TMain is a macro that refers to the entry point of a desktop program (WinMain)
int TMain(TParam) {
	UNREFERENCED_PARAMETER(hNULLInstance);

	//! create a console to direct the I/O stream
	Helper::CreateConsole();

	_cEngine_ eng(
        hInstance, nCmdShow
    );

    //! create a window class and register it in OS
	eng.CreateC(
        (LPWSTR)L"_WndClass_", LoadCursor(NULL, IDC_HAND)
    );

    //! create a overlapping window
	eng.CreateW(
        (LPWSTR)L"[Checkers]",
             nWindowWidth, nWindowHeight, WS_OVERLAPPEDWINDOW | WS_VISIBLE
    );

	DWORD dwResult = ERROR_SUCCESS;

	//! start a loop of dispatching and sending window messages
	dwResult = eng.Start();

	assert(
        dwResult == ERROR_SUCCESS
    );

	return(dwResult);
}
