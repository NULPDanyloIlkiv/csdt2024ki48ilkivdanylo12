#ifndef _DATA_H_
#define _DATA_H_

#define _MSG_SIZE_ 64

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

    _kMESSAGE_ = 0xF
};

typedef uint8_t _KEY_;



//! a structure that stores a data need to initialize the game
struct _sInit_ {
    uint8_t _size_w_, _size_h_;
}; typedef _sInit_ _INIT_;

//! a structure that stores a point with [x y] coordinates
struct _sPoint_ {
    int x, y;
}; typedef _sPoint_ _POINT_;

//! a structure that stores a step | [x y] old -> [x y] new
struct _sStep_ {
    _POINT_ _old_, _new_;
}; typedef _sStep_ _STEP_;

//! structure indicating a checker to be updated on a board | [x y] -> c
struct _sUpdate_ {
    _POINT_ _id_; char _c_;
}; typedef _sUpdate_ _UPDATE_;

#endif // _DATA_H_
