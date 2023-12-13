#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <windows.h>
#include <gl/gl.h>

/**
 * @file
 *
 * @brief opengl.h
 */

//! enable OpenGL for the window
void _EnableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
);

//! disable OpenGL for the window
void _DisableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
);

#endif // _OPENGL_H_
