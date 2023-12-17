#include <cassert>
#include "engine.h"

#include <cmath>

#include <algorithm>

#define _COM_PORT_

#ifdef _COM_PORT_
#define _PORT_ID_ (const wchar_t*)L"COM4"
#else
#define _PORT_ID_ (const wchar_t*)NULL
#endif // _COM_PORT_

/**
 * @file
 *
 * @brief engine.cpp
 *
 * this file stores the implementation of the main class that implements the application
 */

const float _fRADIUS_ = 0.3f;

static bool _is_point_in_circle_(float x0, float y0, float r0, float px, float py) {
    return ((x0 - px) * (x0 - px) + (y0 - py) * (y0 - py)) < (r0 * r0);
}

//! calculate a color (RGB) of a figure by character
static _RGB_ _type_color_(char _f_) {
    _RGB_ _col_ = { 0.f };

    switch(_f_)
    {
    case('W'): case('B'): {
        float _c_ = (_f_ == 'W') ? 0.7f : (_f_ == 'B') ? 0.3f : 0.5f;

        _col_ = {
            _c_, _c_, _c_
        };
    } break;

    case('Q'):
        { _col_ = { 0.1f, 0.7f, 0.1f }; } break;
    case('K'):
        { _col_ = { 0.7f, 0.1f, 0.1f }; } break;

    default: { _col_ = { 0.5f, 0.5f, 0.5f }; } break;

    } return(_col_);
}



//! the engine class constructor
_cEngine_::_cEngine_(const _In_ HINSTANCE& hInst,
    _In_ int nCmdShow
) : _cWindow_(hInst, nCmdShow)
{
    m_bAppActive = 0x1;

    //! create a client handle
    client = std::make_unique<
        _cClient_
    >(_PORT_ID_);

    //! init the game
    client->_game_init_();

    _update_ = true;
}

//! the engine class destructor
_cEngine_::~_cEngine_(void) { /*Code...*/ }



//! a loop of dispatching and sending window messages
DWORD _cEngine_::Start(void) {
    //! variables using to calculate a elapsed time
    std::chrono::system_clock::time_point
        tp0 = std::chrono::system_clock::now(),
        tp1 = std::chrono::system_clock::now();
    std::chrono::duration
        <float> elapsedTime(0.f);



    MSG msg = { 0x0 };

    //! main loop
    while (m_bAppActive) {
        //! check for messages
        if (PeekMessage(&msg, NULL, 0x0, 0x0, PM_REMOVE))
        switch(msg.message) {
            //! user click on the cross - close a window
            case WM_QUIT: {
                m_bAppActive = 0x0;
            } break;

            default: {
                //! handle and dispatch messages
                if (!TranslateAccelerator(msg.hwnd, m_hAccelTable, &msg))
                { TranslateMessage(&msg); DispatchMessage(&msg); }
            } break;

        } else {
            //! calculation of elapsed time
            tp1 = std::chrono::system_clock::now();
            elapsedTime = tp1 - tp0; tp0 = tp1;

            //! do work, I think it's clear
            if (!_DoWork_(elapsedTime.count())) {
                //! do NOT work? - close a window
                PostMessage(m_hWnd,
                    WM_CLOSE, 0x0, 0x0
                );
            }
        }
    }

    //! return error code
    return(
        (int)msg.wParam
    );
}



//! a window procedure function of a overlapping window
LRESULT _cEngine_::HandleMessage(UINT _In_ uMsg,
    WPARAM _In_ wParam, LPARAM _In_ lParam
)
{
    switch (uMsg) {

    //! window has create
    case WM_CREATE: {
        //! enable OpenGL for a window
        _EnableOpenGL_(
            m_hWnd, &opengl.dc, &opengl.glr
        );
    } break;

    //! window has close
    case WM_CLOSE: {
        assert(
            DestroyWindow(m_hWnd)
        );
    } break;

    //! window has destroy
    case WM_DESTROY: {
        //! shutdown OpenGL for a window
        _DisableOpenGL_(
            m_hWnd, &opengl.dc, &opengl.glr
        );

        PostQuitMessage(EXIT_SUCCESS);
    } break;

    case WM_SIZE: {
        //! resize a window
        POINT _s = {
            (LONG)GET_X_LPARAM(lParam), (LONG)GET_Y_LPARAM(lParam)
        }; m_wSize = _s;

        glViewport(
            0x0, 0x0, _s.x, _s.y
        );
    } break;

    /**
     * things that are need to make a window look beautiful (i hate when a window blinking)
     */

    case WM_WINDOWPOSCHANGING: {
        WINDOWPOS* pWP = reinterpret_cast<WINDOWPOS*>(lParam);
            pWP->flags |= SWP_NOCOPYBITS;
    } break;

    case WM_ERASEBKGND: {
        /*Code...*/
    } return(0x1);



    case WM_GETMINMAXINFO: {
        MINMAXINFO* lpMMI = reinterpret_cast
            <MINMAXINFO*>(lParam);

        lpMMI->ptMinTrackSize = { 160, 160 };
    } break;



    //! drawing a OpenGL frame on a window frame
    case WM_PAINT: {
        _DrawFrame_();
    } break;



    //! find a position of a mouse cursor
    case WM_MOUSEMOVE: {
        UNREFERENCED_PARAMETER(wParam);
        int32_t _x = GET_X_LPARAM(lParam),
            _y = GET_Y_LPARAM(lParam);
        m_pMouse = { _x, _y };
    } break;

    /**
     * data input from mouse buttons
     */

    case WM_RBUTTONDOWN: {
        m_bRMK = 0x1;

        /*Code...*/

        m_vecPan = _MousePos_();
    } break;
    case WM_RBUTTONUP: {
        m_bRMK = 0x0;
    } break;

    case WM_LBUTTONDOWN: {
        m_bLMK = 0x1;
    } break;
    case WM_LBUTTONUP: {
        m_bLMK = 0x0;
    } break;

    //! data input from a mouse wheel
    case WM_MOUSEWHEEL: {
        UNREFERENCED_PARAMETER(lParam);

        int16_t _nDelta = GET_WHEEL_DELTA_WPARAM(wParam);

        m_nWheelDelta = _nDelta;
    } break;

    case WM_KEYUP: {
        size_t _nKey = size_t(wParam);

        switch(_nKey)
        {
        case(VK_SPACE): {
            if (select) {
                select = nullptr;
            }

            client->_game_restart_();

            _update_ = true;

        } break;

        default:
            break;
        }
    } break;

    default:
        break;

    }
    return(DefWindowProc(m_hWnd, uMsg, wParam, lParam));
}



//! body of a main loop
bool _cEngine_::_DoWork_(float fElapsedTime) {
    if (_update_) {
        if (
            !_UpdateData_()
        ) { /*Code...*/ }

        _update_ = 0x0;
    }

    //! mouse position in the range: -1.f -> +1.f
    vec2f _vMOUSE_ = _MousePos_();

    /**
     * camera pan and zoom
     */

    if (m_bRMK) {
        const float _fPAN_SPEED_ = 5.f;

        camera.vOffset -= vec2f { _fPAN_SPEED_ } * (
            _vMOUSE_ - m_vecPan
        ) / vec2f{ camera.fScale };

        m_vecPan = _vMOUSE_;
    }

    /**
     * make a step
     */

    if (m_bLMK) {
        vec3f _p_ = { 0.f };

        _Display_To_World_(
            m_pMouse, _p_.x, _p_.y, _p_.z
        );

        vec2f _fCURSOR_ = vec2f { _p_.x, _p_.y };



        _fCURSOR_.x = clamp(
            _fCURSOR_.x, 0.f, _data_.m_nMapW - 0.01f
        );
        _fCURSOR_.y = clamp(
            _fCURSOR_.y, 0.f, _data_.m_nMapH - 0.01f
        );

        size_t _size_ = _data_.m_vecData.size();

        for (auto& o: _data_.m_vecData) {
            if (
                !(o.cType != '.')
            ) { continue; }

            if (
                _is_point_in_circle_(o.vNewPos.x, o.vNewPos.y, _fRADIUS_, _fCURSOR_.x, _fCURSOR_.y)
            )
            {
                if (!select) {
                    select = &o; select->vOldPos = select->vNewPos;
#ifdef _COM_PORT_
                    client->_step_make_(
                        _kSTEP_MAKE_OLD_, { int(select->vOldPos.x), int(select->vOldPos.y) }
                    );
#endif // _COM_PORT_
                } break;
            }
        }
        if (
            select != nullptr
        )
        { select->vNewPos = _fCURSOR_; }
    } else if (select != nullptr) {
#ifdef _COM_PORT_
        client->_step_make_(
            _kSTEP_MAKE_NEW_, { int(select->vNewPos.x), int(select->vNewPos.y) }
        );

        client->_step_take_();

        _update_ = true;
#endif // _COM_PORT_
        select = nullptr;
    }

    if (_update_) {
        if (
            !_UpdateData_()
        ) { /*Code...*/ }

        _update_ = 0x0;
    }



    vec2f vecMouseBeforeZoomPos = _DisplayToWorld_(
        _vMOUSE_.x, _vMOUSE_.y
    );

    /**
     * calculate a camera offset and zoom
     */

    if (
        m_nWheelDelta > 0x0
    ) {
        camera.fScale += camera.fScale * 0.3f;
    } else if (
        m_nWheelDelta < 0x0
    ) {
        camera.fScale -= camera.fScale * 0.3f;
    } m_nWheelDelta = 0x0;

    const float _ZOOM_max_ = 1.5f;
    const float _ZOOM_min_ = 0.5f;

    //! clamp camera zoom
    camera.fScale = clamp(
        camera.fScale, _ZOOM_min_, _ZOOM_max_
    );

    vec2f vecMouseAfterZoomPos = _DisplayToWorld_(
        _vMOUSE_.x, _vMOUSE_.y
    );

    //! offset a camera by zoom
    camera.vOffset += (
        vecMouseBeforeZoomPos - vecMouseAfterZoomPos
    );

    /*int _nKEY_[] = {
        VK_UP, VK_DOWN, VK_RIGHT, VK_LEFT
    };
    bool _bKEY_[] = {
        0x0, 0x0, 0x0, 0x0
    };

    for (
            int i = 0x0; i < 0x4; i++
    ) { _bKEY_[i] = (0x8000 & GetAsyncKeyState(_nKEY_[i])) != 0x0; }

    if (_bKEY_[0x0])
        camera.vOffset.y += 3.f * fElapsedTime;
    if (_bKEY_[0x1])
        camera.vOffset.y -= 3.f * fElapsedTime;
    if (_bKEY_[0x2])
        camera.vOffset.x += 3.f * fElapsedTime;
    if (_bKEY_[0x3])
        camera.vOffset.x -= 3.f * fElapsedTime;*/

    //! clamp camera movement
    camera.vOffset.x = clamp(
        camera.vOffset.x, -0x1 * (int)_data_.m_nMapW, _data_.m_nMapW
    );
    camera.vOffset.y = clamp(
        camera.vOffset.y, -0x1 * (int)_data_.m_nMapH, _data_.m_nMapH
    );

    //! update a window
    (void)InvalidateRect(
        m_hWnd, NULL, NULL
    );

    return(0x1);
}



//! update a data to current one
bool _cEngine_::_UpdateData_(void) {
    _data_ = {
        client->_map_w_(), client->_map_h_(), client->_data_()
    };

    return(0x1);
}



//! transformations of a point in space
/**
 * in this case - coordinates of the mouse cursor
 */

vec2f _cEngine_::_MousePos_(void) {
    vec2f p = { 0x0 };

    //! mouse position in the range: -1.f -> +1.f
    p = {
        m_pMouse.x / (0.5f * m_wSize.x) - 0x1, (m_wSize.y - m_pMouse.y) / (0.5f * m_wSize.y) - 0x1
    };

    return(p);
}

vec2f _cEngine_::_DisplayToWorld_(
    float fDisplayX, float fDisplayY
)
{
    vec2f _fPOINT_ = {
        fDisplayX, fDisplayY
    };

    auto _s_ = camera.fScale;
    _fPOINT_.x /= 2.f / _data_.m_nMapW * _s_;
    _fPOINT_.y /= 2.f / _data_.m_nMapH * _s_;

    auto _o_ = camera.vOffset;
    _fPOINT_.x += 0.5f * _data_.m_nMapW + _o_.x;
    _fPOINT_.y += 0.5f * _data_.m_nMapH + _o_.y;

    return(_fPOINT_);
}



//! drawing a OpenGL frame on a window frame
void _cEngine_::_DrawFrame_(void) {
    glEnable(GL_DEPTH_TEST);

    glClearColor(
        0.1f, 0.1f, 0.1f, 0.f
    );
    glClear(
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
    );

    glMatrixMode(
        GL_PROJECTION
    );
    glLoadIdentity();

    glOrtho(
        -1.f,+1.f, -1.f,+1.f, -1.f, +16.f
    );

    glMatrixMode(
        GL_MODELVIEW
    );
    glLoadIdentity();



    glScalef(
        camera.fScale, camera.fScale, 1.f
    );
    glScalef(
        2.f / _data_.m_nMapW, 2.f / _data_.m_nMapH, 1.f
    );



    glTranslatef(
        _data_.m_nMapW * -0.5f, _data_.m_nMapH * -0.5f, 0.f
    );

    glTranslatef(
        -1.f * camera.vOffset.x, -1.f * camera.vOffset.y, 0.f
    );



    for(size_t y = 0x0; y < _data_.m_nMapH; y += 0x1)
    for(size_t x = 0x0; x < _data_.m_nMapW; x += 0x1) {
        glPushMatrix();
            glTranslatef(
                x, y, -1.f
            );

            bool _COL_ = (x + y) % 0x2;

            _DrawBoard_(_COL_);
        glPopMatrix();
    }
    for(size_t y = 0x0; y <= _data_.m_nMapH; y += 0x1)
    for(size_t x = 0x0; x <= _data_.m_nMapW; x += 0x1) {
        glPushMatrix();

            glTranslatef(
                0.f, 0.f, -0.9f
            );

            glColor3f(
                1.f, 1.f, 1.f
            );

            glLineWidth(0x3);
            _Draw_Polygon_(
                { { x, 0x0 }, { x, _data_.m_nMapH }, { 0x0, y }, { _data_.m_nMapW, y } }, GL_LINES
            );

        glPopMatrix();
    }


    glPushMatrix();

        vec3f _p_ = { 0.f };

        _Display_To_World_(
            m_pMouse, _p_.x, _p_.y, _p_.z
        );

        vec2f _fCURSOR_ = vec2f { _p_.x, _p_.y };

    glPopMatrix();

    if (
        _fCURSOR_.x >= 0x0 && _fCURSOR_.x < _data_.m_nMapW && _fCURSOR_.y >= 0x0 && _fCURSOR_.y < _data_.m_nMapH
    ) {
        glPushMatrix();

            glTranslatef(
                0.f, 0.f, -0.75f
            );

            glColor3f(
                0.9f, 0.9f, 0.1f
            );

            vec2i _iCURSOR_ = (vec2i)_fCURSOR_;

            glLineWidth(0x3);
            _Draw_Polygon_(
                {
                    { _iCURSOR_.x, _iCURSOR_.y }, { _iCURSOR_.x + 0x1, _iCURSOR_.y },
                    { _iCURSOR_.x + 0x1, _iCURSOR_.y + 0x1 }, { _iCURSOR_.x, _iCURSOR_.y + 0x1 }
                }, GL_LINE_LOOP
            );

        glPopMatrix();
    }



    std::vector
        <vec2f> _vert_(0x5);

    for(size_t i = 0x0; i < 0x5; i += 0x1)
        _vert_[i] = vec2f {
            sinf(i * (6.28f / 0x5)), cosf(i * (6.28f / 0x5))
        };

    for (auto& o : _data_.m_vecData) {
        if (
            !(o.cType != '.')
        ) { continue; }

        _RGB_ _col_ = { 0.f };
        _col_ = _type_color_(
            o.cType
        );

        glPushMatrix();

            glTranslatef(
                o.vNewPos.x, o.vNewPos.y, -0.5f
            );

            glScalef(
                1.f * _fRADIUS_, 1.f * _fRADIUS_,  1.f
            );



            glColor3f(
                _col_._r_, _col_._g_, _col_._b_
            );

            _Draw_Polygon_(_vert_, GL_POLYGON);



            glColor3f(
                1.f, 1.f, 1.f
            );

            glLineWidth(0x5);
            _Draw_Polygon_(_vert_, GL_LINE_LOOP);

            glPointSize(0x3);
            _Draw_Polygon_(_vert_, GL_POINTS);

        glPopMatrix();
    }
    if (
        select != nullptr && select->cType != '.'
    )
    {
        _RGB_ _col_ = { 0.f };
        _col_ = _type_color_(
            select->cType
        );

        glPushMatrix();

            glTranslatef(
                select->vNewPos.x, select->vNewPos.y, -0.3f
            );

            glScalef(
                1.5f * _fRADIUS_, 1.5f * _fRADIUS_,  1.f
            );



            glColor3f(
                _col_._r_, _col_._g_, _col_._b_
            );

            _Draw_Polygon_(_vert_, GL_POLYGON);



            glColor3f(
                0.9f, 0.9f, 0.1f
            );

            glLineWidth(0x5);
            _Draw_Polygon_(_vert_, GL_LINE_LOOP);

            glPointSize(0x3);
            _Draw_Polygon_(_vert_, GL_POINTS);

        glPopMatrix();
    }

    SwapBuffers(opengl.dc);
}



void _cEngine_::_DrawBoard_(bool _COL_) {
    vec2f _vert_[] = {
        { 0.f, 0.f }, { 0.f, 1.f }, { 1.f, 1.f }, { 1.f, 0.f }
    };

    float c0 = _COL_ ? 0.9f : 0.1f;
    float c1 = _COL_ ? 0.75f : 0.35f;

    float _col_[] = {
        c0,c0,c0, c0,c0,c0, c1,c1,c1, c1,c1,c1
    };

    glVertexPointer(
        0x2, GL_FLOAT, 0x0, &_vert_
    );

    glEnableClientState(GL_VERTEX_ARRAY);

        glColorPointer(
            0x3, GL_FLOAT, 0x0, &_col_
        );

        glEnableClientState(GL_COLOR_ARRAY);

        glDrawArrays(
            GL_QUADS, 0x0, 0x4
        );

        glDisableClientState(GL_COLOR_ARRAY);

    glDisableClientState(GL_VERTEX_ARRAY);
}
