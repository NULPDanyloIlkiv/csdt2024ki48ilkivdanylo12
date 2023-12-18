#include "data.h"

/**
 * @file
 *
 * @brief data.c
 */

size_t _data_add_(
    _DATA_* _data_, size_t _size_
) {
    _data_->_cnt_ += 0x1;
    _data_->_data_ = realloc(
        _data_->_data_, _data_->_cnt_ * _size_
    );

    return(_data_->_cnt_);
}

size_t _data_clear_(_DATA_* _data_) {
    size_t _size_ = _data_->_cnt_;

    free(_data_->_data_);
    _data_->_data_ = NULL;

    _data_->_cnt_ = 0x0;

    return(_size_);
}
