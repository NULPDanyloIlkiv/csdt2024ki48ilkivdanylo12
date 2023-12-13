#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "framework.h"

#include "window.h"
#include "opengl.h"

#include "client.h"

#include <vector>

/**
 * @file
 *
 * @brief engine.h
 */

static const int nWindowWidth = 640;
static const int nWindowHeight = 640;

/**
 * @brief the engine class
 *
 * this class implements the application
 */
class _cEngine_ :
	public _cWindow_
{
    //! a window procedure function of a overlapping window
	LRESULT HandleMessage(UINT _In_ uMsg,
		WPARAM _In_ wParam, LPARAM _In_ lParam
    );

    //! body of a main loop
	bool _DoWork_(float fElapsedTime);


    //! transformations of a point in space
    /**
     * in this case - coordinates of the mouse cursor
     */

    vec2f _MousePos_(void);

    vec2f _DisplayToWorld_(
        float fDisplayX, float fDisplayY
    );



    //! drawing a OpenGL frame on a window frame
    void _Draw_Frame_(void);

    void _Draw_Board_(bool _COL_);

	void _Draw_Polygon_(
        const std::vector<vec2f>& _data_, int32_t _mode_
    );



    /**
     * class attributes
     */

    //! client handle
    std::unique_ptr<
        _cClient_
    > client = nullptr;



    bool m_bAppActive = 0x0;



    //! a structure that stores the descriptor of the output device, and the rendering device
    struct _sOpenGL_ {
        HDC dc = NULL; HGLRC glr = { 0x0 };
    } opengl;

    //! a structure that stores attributes needed to work with a camera
    struct _sCamera_ {
        vec2f vOffset = { 0x0 }; float fScale = 1.f;
    } camera;

    vec2f m_vecPan = { 0x0 };



    //! attributes used for a keyboard and a mouse input
    POINT m_pMouse = { 0x0 };
    bool m_bLMK = 0x0, m_bRMK = 0x0;
    int16_t m_nWheelDelta = 0x0;



    //! pointer to the object we are working with
    _sObject_* select = nullptr;



public:

    /**
     * class constructor / destructor
     */

	_cEngine_(const _In_ HINSTANCE& hInst,
		_In_ int nCmdShow = SW_SHOWNORMAL
	); ~_cEngine_(void);

	//! a loop of dispatching and sending window messages
	DWORD Start(void);
};

#endif // _ENGINE_H_
