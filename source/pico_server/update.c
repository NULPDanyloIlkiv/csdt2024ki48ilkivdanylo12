#include "update.h"

#include "serial_port.h"

/**
 * @file
 *
 * @brief update.c
 */

//! structure indicating a checker to be updated on a board | [x y] -> c
struct _sUpdate_ {
    _POINT_ _id_; char _c_;
}; typedef struct _sUpdate_ _UPDATE_;

//! a structure that stores a array of structures _UPDATE_
static struct {
    size_t _cnt_; _UPDATE_* _data_;
} _update_ = { 0x0, NULL };



size_t _update_add_(
    _POINT_ _point_, char _c_
)
{
    _update_._cnt_ += 0x1;
    _update_._data_ = realloc(
        _update_._data_, _update_._cnt_ * sizeof(_UPDATE_)
    );

    _update_._data_[
        _update_._cnt_ - 0x1
    ] = (_UPDATE_){ _point_, _c_ };

    return(_update_._cnt_);
}

size_t _update_clear_(void) {
    size_t _size_ = _update_._cnt_;

    free(_update_._data_);
    _update_._data_ = NULL;

    _update_._cnt_ = 0x0;

    return(_size_);
}

size_t _update_send_(void) {
    size_t _size_ = 0x0;

    _size_ += _com_send_data_(
        _kUPDATE_, &_update_._cnt_, 0x1, sizeof(_update_._cnt_)
    );

    if (
        _update_._data_ != NULL
    )
    {
        _size_ += _com_send_(
           _update_._data_, _update_._cnt_, sizeof(_UPDATE_)
        );
    }

    return(_size_);
}
