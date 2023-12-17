#include "logic.h"

#include "game.h"

#include "update.h"

/**
 * @file
 *
 * @brief logic.c
 */

void* _logic_all_(
    void* _data_, _POINT_ _id_, char _c_, int8_t _set_, void* (_f_)(void*, _POINT_, char, _POINT_, int8_t, int8_t)
) {
    if (_set_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set_;

            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _data_ = _f_(
                _data_, _id_, _c_, _p_, x, y
            );
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _data_ = _f_(
                _data_, _id_, _c_, _p_, x, y
            );
        }
    }

    return(_data_);
}



_MASK_MOVE_ _logic_create_mask_move_(
    _MASK_MOVE_ _data_
)
{
    _data_._data_ = malloc(
        sizeof(bool*) * _data_._w_
    );

    if (
        !_data_._data_
    ) { return(_data_); }

    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    )
    {
        _data_._data_[i] = calloc(
            _data_._h_, sizeof(bool)
        );

        if (
            !_data_._data_[i]
        )
        {
            for (
                 size_t j = 0x0; j < i; j += 0x1
            ) { free(_data_._data_[j]); _data_._data_[j] = NULL; }

            free(_data_._data_); _data_ = (_MASK_MOVE_){};
        }
    }

    return(_data_);
}

_MASK_MOVE_ _logic_clear_mask_move_(
    _MASK_MOVE_ _data_
)
{
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { _data_._data_[x][y] = NULL; }

    _data_._cnt_ = 0x0;

    return(_data_);
}

_MASK_MOVE_ _logic_destroy_mask_move_(
    _MASK_MOVE_ _data_
)
{
    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    ) { free(_data_._data_[i]); _data_._data_[i] = NULL; }

    free(_data_._data_); _data_ = (_MASK_MOVE_){};

    return(_data_);
}



static void* _logic_move_(
    void* _data_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    _MASK_MOVE_* _mask_move_ = (_MASK_MOVE_*)_data_;

    if (
        _is_inside_board_(_p_.x, _p_.y)
    ) {
        _mask_move_->_data_[_p_.x][_p_.y] = _logic_is_move_one_(
            _id_, _c_, _p_
        );

        _mask_move_->_cnt_ += 0x1;
    }

    return(_data_);
}



bool _logic_is_move_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
) {
    if (
        _is_inside_board_(_p_.x, _p_.y)
    )
    {
        return(
            _get_board_char_(_p_.x, _p_.y) == '.'
        );
    } else {
        return(0x0);
    }
}

_MASK_MOVE_ _logic_find_move_all_(
    _MASK_MOVE_ _data_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = _game_set_(_c_);

    (void)_logic_all_(
        &_data_, _id_, _c_, _set_, &_logic_move_
    );

    return(_data_);
}



_MASK_JUMP_ _logic_create_mask_jump_(
    _MASK_JUMP_ _data_
)
{
    _data_._data_ = malloc(
        sizeof(_POINT_**) * _data_._w_
    );

    if (
        !_data_._data_
    ) { return(_data_); }

    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    )
    {
        _data_._data_[i] = calloc(
            _data_._h_, sizeof(_POINT_*)
        );

        if (
            !_data_._data_[i]
        )
        {
            for (
                 size_t j = 0x0; j < i; j += 0x1
            ) { free(_data_._data_[j]); _data_._data_[j] = NULL; }

            free(_data_._data_); _data_ = (_MASK_JUMP_){};
        }
    }

    return(_data_);
}

_MASK_JUMP_ _logic_clear_mask_jump_(
    _MASK_JUMP_ _data_
)
{
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { free(_data_._data_[x][y]); _data_._data_[x][y] = NULL; }

    _data_._cnt_ = 0x0;

    return(_data_);
}

_MASK_JUMP_ _logic_destroy_mask_jump_(
    _MASK_JUMP_ _data_
)
{
    for (
        int8_t i = 0x0; i < _data_._w_; i += 0x1
    ) { free(_data_._data_[i]); _data_._data_[i] = NULL; }

    free(_data_._data_); _data_ = (_MASK_JUMP_){};
}



static void* _logic_jump_(
    void* _data_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    _MASK_JUMP_* _mask_jump_ = (_MASK_JUMP_*)_data_;

    if (
        _logic_is_jump_one_(_id_, _c_, _p_)
    ) {
        _mask_jump_->_data_[_p_.x + x][_p_.y + y] = malloc(
            sizeof(_POINT_)
        );

        *_mask_jump_->_data_
            [_p_.x + x][_p_.y + y] = _p_;

        _mask_jump_->_cnt_ += 0x1;
    }

    return(_data_);
}



bool _logic_is_jump_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
) {
    int x = MAX(MIN(_p_.x - _id_.x, +0x1), -0x1);
    int y = MAX(MIN(_p_.y - _id_.y, +0x1), -0x1);

    if (
        _is_inside_board_(_p_.x, _p_.y)
    )
    {
        char _t_ = _get_board_char_(
            _p_.x, _p_.y
        );

        if (
            _is_inside_board_(_p_.x + x, _p_.y + y)
        )
        {
            bool _dot_ = _get_board_char_(_p_.x + x, _p_.y + y) == '.';

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

_MASK_JUMP_ _logic_find_jump_all_(
    _MASK_JUMP_ _data_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = _game_set_(_c_);

    (void)_logic_all_(
        &_data_, _id_, _c_, _set_, &_logic_jump_
    );

    return(_data_);
}



static void* _logic_is_jump_(
    void* _data_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    bool* _b_ = (bool*)_data_;

    *(_b_) |= _logic_is_jump_one_(_id_, _c_, _p_);

    return(_data_);
}

bool _logic_is_jump_all_(
    _POINT_ _id_, char _c_
) {
    bool _b_ = 0x0;

    int8_t _set_ = _game_set_(_c_);

    (void)_logic_all_(
        &_b_, _id_, _c_, _set_, &_logic_is_jump_
    );

    return(_b_);
}

bool _logic_is_jump_at_least_all_(bool _turn_) {
    bool _b_ = 0x0;

    for (int8_t y = 0x0; y < _get_board_h_(); y += 0x1)
    for (int8_t x = 0x0; x < _get_board_w_(); x += 0x1)
    {
        char _c_ = _get_board_char_(x, y);

        if (
            !(_turn_) ? (_c_ != 'W' && _c_ != 'Q') : (_c_ != 'B' && _c_ != 'K')
        ) { continue; }

        int8_t _set_ = _game_set_(_c_);

        (void)_logic_all_(
            &_b_, (_POINT_){ x, y }, _c_, _set_, &_logic_is_jump_
        );

        if (_b_) { return(_b_); }
    }

    return(_b_);
}



static _MASK_MOVE_
    _mask_move_ = {};

static _MASK_JUMP_
    _mask_jump_ = {};

bool _logic_create_(void) {
    _mask_move_ = (_MASK_MOVE_){};
    _mask_jump_ = (_MASK_JUMP_){};

    bool _b_ = 0x0;

    if (
        _game_create_()
    )
    {
        _POINT_ _point_ = {
            _get_board_w_(), _get_board_h_()
        };

        _mask_move_._w_ = _point_.x;
        _mask_move_._h_ = _point_.y;

        _mask_move_ = _logic_create_mask_move_(
            _mask_move_
        );

        _mask_jump_._w_ = _point_.x;
        _mask_jump_._h_ = _point_.y;

        _mask_jump_ = _logic_create_mask_jump_(
            _mask_jump_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

bool _logic_destroy_(void) {
    bool _b_ = 0x0;

    if (
        _game_destroy_()
    )
    {
        _mask_move_ = _logic_destroy_mask_move_(
            _mask_move_
        );
        _mask_jump_ = _logic_destroy_mask_jump_(
            _mask_jump_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

bool _logic_step_(
    _POINT_ _old_, _POINT_ _new_, _STEP_* _step_
) {
    if (
        _mask_move_._data_ != NULL && _mask_jump_._data_ != NULL
    )
    {
        char _c_ = _get_board_char_(
            _old_.x, _old_.y
        );

        if (
            !_game_turn_(_c_, 0x0)
        ) {
            _new_ = _old_;

            goto linkExit;
        }

        _mask_jump_ = _logic_clear_mask_jump_(_mask_jump_);

        _mask_jump_ = _logic_find_jump_all_(
            _mask_jump_, _old_, _c_
        );

        if (_mask_jump_._cnt_ != 0x0) {
            if (
                _mask_jump_._data_[_new_.x][_new_.y] != NULL
            ) {
                if (_get_flag_() & _fCOMBO_) {
                    _STEP_ _s_ = _get_mem_();

                    if (
                        !(_s_._new_.x == _old_.x && _s_._new_.y == _old_.y)
                    ) {
                        _new_ = _old_;

                        goto linkExit;
                    }
                }

                _game_step_(
                    (_STEP_){ _old_, _new_ }
                );

                _POINT_* _jump_ = _mask_jump_._data_
                    [_new_.x][_new_.y];

                _POINT_ _j_ = *(_jump_);

                _set_board_char_(
                    _j_.x, _j_.y, '.'
                );

                (void)_update_add_(_j_, '.');



                if (
                    _game_q_or_k_(_new_, true)
                ) {
                    _c_ = _get_board_char_(
                        _new_.x, _new_.y
                    );
                }

                bool _b_ = _logic_is_jump_all_(
                    _new_, _c_
                );

                int8_t _f_ = _get_flag_();
                _b_ ? (_f_ |= _fCOMBO_) : (_f_ &= ~(_fCOMBO_));
                (void)_set_flag_(_f_);

                (void)_game_turn_(_c_, !_b_);
            } else {
                _new_ = _old_; goto linkExit;
            }
        } else if (
            !_logic_is_jump_at_least_all_(_get_flag_() & _fTURN_)
        ) {
            _mask_move_ = _logic_clear_mask_move_(_mask_move_);

            _mask_move_ = _logic_find_move_all_(
                _mask_move_, _old_, _c_
            );

            if (
                _mask_move_._data_[_new_.x][_new_.y]
            ) {
                _game_step_(
                    (_STEP_){ _old_, _new_ }
                );

                (void)_game_turn_(_c_, true);
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
        _game_q_or_k_(_new_, true)
    )
    {
        (void)_update_add_(
            _new_, _get_board_char_(_new_.x, _new_.y)
        );
    }

    _set_mem_(
        (_STEP_){ _old_, _new_ }
    );

linkExit:
    *(_step_) = (_STEP_){
        _old_, _new_
    };

    return(true);
}
