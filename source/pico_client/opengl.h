#ifndef _OPENGL_H_
#define _OPENGL_H_

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/**
 * @file
 *
 * @brief opengl.h
 */

 struct _sRGB_{
    float _r_, _g_, _b_;
}; typedef _sRGB_ _RGB_;

//! enable OpenGL for the window
void _EnableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
);

//! disable OpenGL for the window
void _DisableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
);

void _Display_To_World_(
    POINT _in_, float& _out_x_, float& _out_y_, float& _out_z_
);

#include "framework.h"

#include <vector>

void _Draw_Polygon_(
    const std::vector<vec2f>& _data_, int32_t _mode_
);

#endif // _OPENGL_H_
