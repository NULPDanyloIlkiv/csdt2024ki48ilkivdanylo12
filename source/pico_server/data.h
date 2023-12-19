#ifndef _DATA_H_
#define _DATA_H_

#ifndef _MSG_SIZE_
#define _MSG_SIZE_ 64
#endif // _MSG_SIZE_

#include "framework.h"

/**
 * @file
 *
 * @brief data.h
 *
 * this file stores declaration of structures using to store messages
 */

//! a message -> [key + data]
enum _eKey_ {
    _kERROR_ = 0x0, _kINIT_ = 0x1, _kTURN_ = 0x2, _kSTEP_MAKE_ = 0x3, _kSTEP_BOT_ = 0x4, _kSTEP_TAKE_ = 0x5, _kUPDATE_ = 0x6,

    _kMESSAGE_ = 0x7, _kRESTART_ = 0x8
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



struct _sBoard_ {
    uint8_t _w_, _h_; char* _data_;
};

typedef struct _sBoard_ _BOARD_;

bool _is_inside_board_(
    _BOARD_ _board_, int x, int y
);

char _get_board_char_(
    _BOARD_ _board_, uint8_t x, uint8_t y
);

char _set_board_char_(
    _BOARD_ _board_, uint8_t x, uint8_t y, char _new_
);



struct _sData_ {
    size_t _cnt_; void* _data_;
}; typedef struct _sData_ _DATA_;

size_t _data_add_(
    _DATA_* _data_, size_t _size_
);

size_t _data_clear_(_DATA_* _data_);

#endif // _DATA_H_
