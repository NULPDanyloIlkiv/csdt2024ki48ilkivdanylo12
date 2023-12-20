#include "update.h"

/**
 * @file
 *
 * @brief update.c
 */

size_t _update_add_(
    _DATA_* _data_, _CHECKER_ _ch_old_, _CHECKER_ _ch_new_
)
{
    (void)_data_add_(
        _data_, sizeof(_UPDATE_)
    );

    _UPDATE_* _update_ = (_UPDATE_*)_data_->_data_;

    _update_[
        _data_->_cnt_ - 0x1
    ] = (_UPDATE_){ _ch_old_, _ch_new_ };

    return(_data_->_cnt_);
}

size_t _update_clear_(_DATA_* _data_) {
    return(
        _data_clear_(_data_)
    );
}
