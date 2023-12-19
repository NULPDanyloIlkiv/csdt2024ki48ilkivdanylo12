#include "logic.h"

/**
 * @file
 *
 * @brief logic.c
 */

//! a direction of move | 0x0 - ALL | +0x1 - UP | -0x1 - DOWN
int8_t _logic_set_move_(char _c_) {
    return((_c_ == 'W') ? 0x1 : (_c_ == 'B') ? -0x1 : '\0');
}
//! a direction of jump | 0x0 - ALL | +0x1 - UP | -0x1 - DOWN
int8_t _logic_set_jump_(char _c_) {
    return((_c_ == 'W') ? 0x1 : (_c_ == 'B') ? -0x1 : '\0');
}

void* _logic_all_(
    void* _data_, _GAME_* _game_, _POINT_ _id_, char _c_, int8_t _set_, void* (_f_)(void*, _GAME_*, _POINT_, char, _POINT_, int8_t, int8_t)
) {
    if (_set_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set_;

            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _data_ = _f_(
                _data_, _game_, _id_, _c_, _p_, x, y
            );
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _data_ = _f_(
                _data_, _game_, _id_, _c_, _p_, x, y
            );
        }
    }

    return(_data_);
}



_MASK_ _logic_create_mask_(
    _MASK_ _data_, size_t _size_
)
{
    _data_._data_ = malloc(
        sizeof(void*) * _data_._w_
    );

    if (
        !_data_._data_
    ) { return(_data_); }

    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    )
    {
        _data_._data_[i] = calloc(
            _data_._h_, _size_
        );

        if (
            !_data_._data_[i]
        )
        {
            for (
                 size_t j = 0x0; j < i; j += 0x1
            ) { free(_data_._data_[j]); _data_._data_[j] = NULL; }

            free(_data_._data_); _data_ = (_MASK_){};
        }
    }

    return(_data_);
}

_MASK_ _logic_destroy_mask_(
    _MASK_ _data_
)
{
    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    ) { free(_data_._data_[i]); _data_._data_[i] = NULL; }

    free(_data_._data_); _data_ = (_MASK_){};

    return(_data_);
}



static _MASK_ _logic_clear_move_(_MASK_ _data_) {
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { ((bool**)(_data_._data_))[x][y] = 0x0; }

    _data_._cnt_ = 0x0;

    return(_data_);
}



static void* _logic_move_(
    void* _data_, _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    _MASK_* _mask_ = (_MASK_*)_data_;

    if (
        _is_inside_board_(_game_->_board_, _p_.x, _p_.y)
    ) {
        ((bool**)(_mask_->_data_))[_p_.x][_p_.y] = _logic_is_move_one_(
            _game_, _id_, _c_, _p_
        );

        _mask_->_cnt_ += 0x1;
    }

    return(_data_);
}



bool _logic_is_move_one_(
    _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_
) {
    if (
        _is_inside_board_(_game_->_board_, _p_.x, _p_.y)
    )
    {
        return(
            _get_board_char_(_game_->_board_, _p_.x, _p_.y) == '.'
        );
    } else {
        return(0x0);
    }
}

_MASK_ _logic_find_move_all_(
    _MASK_ _data_, _GAME_* _game_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = _logic_set_move_(_c_);

    (void)_logic_all_(
        &_data_, _game_, _id_, _c_, _set_, &_logic_move_
    );

    return(_data_);
}



static _MASK_ _logic_clear_jump_(
    _MASK_ _data_
)
{
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { free(((_POINT_***)(_data_._data_))[x][y]); ((_POINT_***)(_data_._data_))[x][y] = NULL; }

    _data_._cnt_ = 0x0;

    return(_data_);
}



static void* _logic_jump_(
    void* _data_, _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    _MASK_* _mask_ = (_MASK_*)_data_;

    if (
        _logic_is_jump_one_(_game_, _id_, _c_, _p_)
    ) {
        ((_POINT_***)(_mask_->_data_))[_p_.x + x][_p_.y + y] = malloc(
            sizeof(_POINT_)
        );

        *((_POINT_***)(_mask_->_data_))
            [_p_.x + x][_p_.y + y] = _p_;

        _mask_->_cnt_ += 0x1;
    }

    return(_data_);
}



bool _logic_is_jump_one_(
    _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_
) {
    int x = MAX(MIN(_p_.x - _id_.x, +0x1), -0x1);
    int y = MAX(MIN(_p_.y - _id_.y, +0x1), -0x1);

    if (
        _is_inside_board_(_game_->_board_, _p_.x, _p_.y)
    )
    {
        char _t_ = _get_board_char_(
            _game_->_board_, _p_.x, _p_.y
        );

        if (
            _is_inside_board_(_game_->_board_, _p_.x + x, _p_.y + y)
        )
        {
            bool _dot_ = _get_board_char_(_game_->_board_, _p_.x + x, _p_.y + y) == '.';

            return(
                _dot_ && _t_ != '.' && _c_ != _t_ && _c_ != (
                    _t_ == 'W' ? 'Q' : _t_ == 'Q' ? 'W' : _t_ == 'B' ? 'K' : _t_ == 'K' ? 'B' : '\0'
                )
            );
        } else {
            return(0x0);
        }
    } else {
        return(0x0);
    }
}

_MASK_ _logic_find_jump_all_(
    _MASK_ _data_, _GAME_* _game_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = _logic_set_jump_(_c_);

    (void)_logic_all_(
        &_data_, _game_, _id_, _c_, _set_, &_logic_jump_
    );

    return(_data_);
}



static void* _logic_is_jump_(
    void* _data_, _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    bool* _b_ = (bool*)_data_;

    *(_b_) |= _logic_is_jump_one_(_game_, _id_, _c_, _p_);

    return(_data_);
}

bool _logic_is_jump_all_(
    _GAME_* _game_, _POINT_ _id_, char _c_
) {
    bool _b_ = 0x0;

    int8_t _set_ = _logic_set_jump_(_c_);

    (void)_logic_all_(
        &_b_, _game_, _id_, _c_, _set_, &_logic_is_jump_
    );

    return(_b_);
}

bool _logic_is_jump_at_least_all_(
    _GAME_* _game_, bool _turn_
) {
    bool _b_ = 0x0;

    for (int8_t y = 0x0; y < _game_->_board_._h_; y += 0x1)
    for (int8_t x = 0x0; x < _game_->_board_._h_; x += 0x1)
    {
        char _c_ = _get_board_char_(_game_->_board_, x, y);

        if (
            !(_turn_) ? (_c_ != 'W' && _c_ != 'Q') : (_c_ != 'B' && _c_ != 'K')
        ) { continue; }

        int8_t _set_ = _logic_set_jump_(_c_);

        (void)_logic_all_(
            &_b_, _game_, (_POINT_){ x, y }, _c_, _set_, &_logic_is_jump_
        );

        if (_b_) { return(_b_); }
    }

    return(_b_);
}



#include "update.h"

static _MASK_
    _mask_move_ = {};

static _MASK_
    _mask_jump_ = {};

bool _logic_create_(_GAME_* _game_) {
    bool _b_ = 0x0;

    if (
        _game_ != NULL
    )
    {
        _POINT_ _point_ = {
            _game_->_board_._w_, _game_->_board_._h_
        };

        _mask_move_._w_ = _point_.x;
        _mask_move_._h_ = _point_.y;

        _mask_move_ = _logic_create_mask_(
            _mask_move_, sizeof(bool)
        );

        _mask_jump_._w_ = _point_.x;
        _mask_jump_._h_ = _point_.y;

        _mask_jump_ = _logic_create_mask_(
            _mask_jump_, sizeof(_POINT_*)
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

bool _logic_destroy_(_GAME_* _game_) {
    bool _b_ = 0x0;

    if (
        _game_ != NULL
    )
    {
        _mask_move_ = _logic_destroy_mask_(
            _mask_move_
        );
        _mask_move_ = (_MASK_){};

        _mask_jump_ = _logic_destroy_mask_(
            _mask_jump_
        );
        _mask_jump_ = (_MASK_){};

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

bool _logic_step_(
    _GAME_* _game_, _POINT_ _old_, _POINT_ _new_, _STEP_* _step_
) {
    if (
        _get_board_char_(
            _game_->_board_, _old_.x, _old_.y
        ) == '.'
    ) { return(0x0); }

    if (
        _mask_move_._data_ != NULL && _mask_jump_._data_ != NULL
    )
    {
        char _c_ = _get_board_char_(
            _game_->_board_, _old_.x, _old_.y
        );

        if (
            !_game_turn_(_game_, _c_, 0x0)
        ) {
            _new_ = _old_;

            goto linkExit;
        }

        _mask_jump_ = _logic_clear_jump_(_mask_jump_);

        _mask_jump_ = _logic_find_jump_all_(
            _mask_jump_, _game_, _old_, _c_
        );

        if (_mask_jump_._cnt_ != 0x0) {
            if (
                ((_POINT_***)(_mask_jump_._data_))[_new_.x][_new_.y] != NULL
            ) {
                if (_game_->_flag_i_ & _fCOMBO_) {
                    _STEP_ _mem_ = _game_->_mem_;

                    if (
                        !(_old_.x == _mem_._new_.x && _old_.y == _mem_._new_.y)
                    ) {
                        _new_ = _old_;

                        goto linkExit;
                    }
                }

                _game_step_(
                    _game_, (_STEP_){ _old_, _new_ }
                );

                _POINT_* _jump_ = ((_POINT_***)(_mask_jump_._data_))
                    [_new_.x][_new_.y];

                _POINT_ _j_ = *(_jump_);

                _set_board_char_(
                    _game_->_board_, _j_.x, _j_.y, '.'
                );

                (void)_update_add_(_j_, '.');



                if (
                    _game_q_or_k_(_game_, _new_, true)
                ) {
                    _c_ = _get_board_char_(
                        _game_->_board_, _new_.x, _new_.y
                    );
                }

                bool _b_ = _logic_is_jump_all_(
                    _game_, _new_, _c_
                );

                int8_t _f_ = _game_->_flag_i_;
                _b_ ? (_f_ |= _fCOMBO_) : (_f_ &= ~(_fCOMBO_));
                _game_->_flag_i_ = _f_;

                (void)_game_turn_(_game_, _c_, !_b_);
            } else {
                _new_ = _old_; goto linkExit;
            }
        } else if (
            !_logic_is_jump_at_least_all_(_game_, _game_->_flag_i_ & _fTURN_)
        ) {
            _mask_move_ = _logic_clear_move_(_mask_move_);

            _mask_move_ = _logic_find_move_all_(
                _mask_move_, _game_, _old_, _c_
            );

            if (
                ((bool**)(_mask_move_._data_))[_new_.x][_new_.y]
            ) {
                _game_step_(
                    _game_, (_STEP_){ _old_, _new_ }
                );

                (void)_game_turn_(_game_, _c_, true);
            } else {
                _new_ = _old_; goto linkExit;
            }
        } else {
            _new_ = _old_; goto linkExit;
        }
    } else {
        return(0x0);
    }

    if (
        _game_q_or_k_(_game_, _new_, true)
    )
    {
        (void)_update_add_(
            _new_, _get_board_char_(_game_->_board_, _new_.x, _new_.y)
        );
    }

    _game_->_mem_ = (_STEP_){ _old_, _new_ };

linkExit:
    *(_step_) = (_STEP_){
        _old_, _new_
    };

    return(true);
}
