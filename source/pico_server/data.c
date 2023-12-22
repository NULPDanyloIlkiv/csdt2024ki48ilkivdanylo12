#include "data.h"

/**
 * @file
 *
 * @brief data.c
 */

bool _is_inside_board_(
    _BOARD_ _board_, int x, int y
) {
    return(
        x >= 0x0 && x < _board_._w_ && y >= 0x0 && y < _board_._h_
    );
}

char _get_board_char_(
    _BOARD_ _board_, uint8_t x, uint8_t y
)
{
    return(
        _board_._data_[x + (y * _board_._w_)]
    );
}

char _set_board_char_(
    _BOARD_ _board_, uint8_t x, uint8_t y, char _new_
)
{
    size_t i = x + (
        y * _board_._w_
    );

    char _old_ = '.';
    _old_ = _board_._data_[i];

    _board_._data_[i] = _new_;

    return(_old_);
}



size_t _data_add_(
    _DATA_* _data_, size_t _size_
) {
    _data_->_data_ = realloc(
        _data_->_data_, (_data_->_cnt_ += 0x1) * _size_
    );

    return(_data_->_cnt_);
}

size_t _data_sub_(
	_DATA_* _data_, size_t _size_
) {
    _data_->_data_ = realloc(
        _data_->_data_, (_data_->_cnt_ -= 0x1) * _size_
    );

    return(_data_->_cnt_);
}

size_t _data_clear_(
    _DATA_* _data_
) {
    size_t _size_ = _data_->_cnt_;

    free(_data_->_data_);
    _data_->_data_ = NULL;

    _data_->_cnt_ = 0x0;

    return(_size_);
}
