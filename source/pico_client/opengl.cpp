#include "opengl.h"

/**
 * @file
 *
 * @brief opengl.cpp
 */

//! enable OpenGL for the window
void _EnableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    //! get the device context (DC)
    *hDC = GetDC(hwnd);

    //! set the pixel format for the DC
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    //! create and enable the render context (RC)
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

//! disable OpenGL for the window
void _DisableOpenGL_(
    HWND hwnd, HDC* hDC, HGLRC* hRC
)
{
    wglMakeCurrent(
        NULL, NULL
    );
    wglDeleteContext(*hRC);

    ReleaseDC(hwnd, *hDC);
}
