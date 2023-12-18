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



void _Display_To_World_(
    POINT _in_, float& _out_x_, float& _out_y_, float& _out_z_
)
{
    int vp[0x4] = {};

    double mat_mod[16] = {}, mat_prj[16] = {};

    glGetIntegerv(GL_VIEWPORT, vp);
    _in_.y = vp[0x3] - _in_.y - 0x1;

    glGetDoublev(GL_MODELVIEW_MATRIX, mat_mod);
    glGetDoublev(GL_PROJECTION_MATRIX, mat_prj);

    double z = 0.f;

    glReadPixels(
        _in_.x, _in_.y, 0x1, 0x1, GL_DEPTH_COMPONENT, GL_DOUBLE, &z
    );

    double _in_x_, _in_y_, _in_z_;

    gluUnProject(_in_.x, _in_.y, z,
        mat_mod, mat_prj, vp, &_in_x_, &_in_y_, &_in_z_
    );

    _out_x_ = _in_x_, _out_y_ = _in_y_, _out_z_ = _in_z_;
}

void _Draw_Polygon_(
    const std::vector<vec2f>& _data_, int32_t _mode_
)
{
    glVertexPointer(
        0x2, GL_FLOAT, 0x0, _data_.data()
    );

    glEnableClientState(GL_VERTEX_ARRAY);
        glDrawArrays(
            _mode_, 0x0, _data_.size()
        );
    glDisableClientState(GL_VERTEX_ARRAY);
}
