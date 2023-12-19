#include "game.h"

/**
 * @file
 *
 * @brief game.c
 */

//! make a step on a board
void _game_step_(
    _GAME_* _game_, _STEP_ _step_
) {
    char _c_ = '.';

    _POINT_* _p_ = {
        &_step_._old_, &_step_._new_
    };

    for (
        size_t i = 0x0; i < 0x2; i += 0x1
    ) { _c_ = _set_board_char_(_game_->_board_, _p_[i].x, _p_[i].y, _c_); }
}



//! make a mortal man queen or king
bool _game_q_or_k_(
    _GAME_* _game_, _POINT_ _point_, bool _s_
) {
    char _c_ = _get_board_char_(
        _game_->_board_,  _point_.x, _point_.y
    );

    bool _b_ = (
        _point_.y == ((_c_ == 'W') || (_c_ == 'Q') ? (_c_ = 'Q', _game_->_board_._h_ - 0x1) : (_c_ == 'B') || (_c_ == 'K') ? (_c_ = 'K', 0x0) : -0x1)
    );

    if (_s_ && _b_)
        _set_board_char_(
            _game_->_board_, _point_.x, _point_.y, _c_
        );

    return(_b_);
}



//! turn to make a step
bool _game_turn_(
    _GAME_* _game_, char _c_, bool _s_
) {
    bool _b_ = !(_game_->_flag_._turn_) ? (_c_ == 'W' || _c_ == 'Q') : (_c_ == 'B' || _c_ == 'K');

    if (_s_ && _b_) {
        _game_->_flag_._turn_ = !_game_->_flag_._turn_;
    }

    return(_b_);
}



//! allocate memory and initialize attributes
bool _game_create_(
    _GAME_* _game_, uint8_t _w_, uint8_t _h_, const char* _data_
)
{
    const size_t
        _size_ = _w_ * _h_;



    _game_->_board_._w_ = _w_, _game_->_board_._h_ = _h_;

    _game_->_board_._data_ = calloc(
        _size_, sizeof(char)
    );

    memcpy(
        _game_->_board_._data_, _data_, _size_
    );



    return(
        _game_->_board_._data_ != NULL
    );
}

//! free allocated memory
bool _game_destroy_(_GAME_* _game_) {
    bool _b_ = 0x0;

    if (
        _game_->_board_._data_ != NULL
    ) {
        free(
            _game_->_board_._data_
        ); _game_->_board_._data_ = NULL;

        *(_game_) = (_GAME_){};

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}
