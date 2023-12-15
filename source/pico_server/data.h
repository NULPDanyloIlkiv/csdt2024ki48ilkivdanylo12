#ifndef _DATA_H_
#define _DATA_H_

#ifndef _MSG_SIZE_
#define _MSG_SIZE_ 64
#endif // _MSG_SIZE_

/**
 * @file
 *
 * @brief data.h
 *
 * this file stores declaration of structures using to store messages
 */

//! a message -> [key + data]
enum _eKey_ {
    _kERROR_ = 0x0, _kINIT_ = 0x1, _kSTEP_MAKE_OLD_ = 0x2, _kSTEP_MAKE_NEW_ = 0x3, _kSTEP_BOT_ = 0x4, _kSTEP_TAKE_ = 0x5, _kUPDATE_ = 0xA,

    _kMESSAGE_ = 0xF, _kRESTART_ = 0xFF
};

typedef uint8_t _KEY_;

//! a structure that stores a data need to initialize the game
struct _sInit_ {
    uint8_t _size_w_, _size_h_;
}; typedef struct _sInit_ _INIT_;

//! a structure that stores a point with [x y] coordinates
struct _sPoint_ {
    int x, y;
}; typedef struct _sPoint_ _POINT_;

//! a structure that stores a step | [x y] old -> [x y] new
struct _sStep_ {
    _POINT_ _old_, _new_;
}; typedef struct _sStep_ _STEP_;



//! board size N x N
static size_t const _gSIZE_W_ = 0x8;
static size_t const _gSIZE_H_ = 0x8;

//! board data N x N
static char* const _gDATA_ = "W.Q.W.W."
                             ".W.W.W.W"
                             "W.W.Q.W."
                             "........"
                             "........"
                             ".B.K.B.B"
                             "B.B.B.B."
                             ".B.B.K.B";

#endif // _DATA_H_
