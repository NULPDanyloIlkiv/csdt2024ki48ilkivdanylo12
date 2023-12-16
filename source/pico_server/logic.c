#include "logic.h"

#include "game.h"

#include "update.h"

/**
 * @file
 *
 * @brief logic.c
 */

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

_MASK_MOVE_ _logic_find_move_(
    _MASK_MOVE_ _data_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = (_c_ == 'W') ? 0x1 : _c_ == 'B' ? -0x1 : 0x0;

    if (_set_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set_;

            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            if (
                _is_inside_board_(_p_.x, _p_.y)
            ) {
                _data_._data_[_p_.x][_p_.y] = _logic_is_move_one_(_id_, _c_, _p_);

                _data_._cnt_ += 0x1;
            }
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            if (
                _is_inside_board_(_p_.x, _p_.y)
            ) {
                _data_._data_[_p_.x][_p_.y] = _logic_is_move_one_(_id_, _c_, _p_);

                _data_._cnt_ += 0x1;
            }
        }
    }

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

_MASK_JUMP_ _logic_find_jump_(
    _MASK_JUMP_ _data_, _POINT_ _id_, char _c_
)
{
    int8_t _set_ = (_c_ == 'W') ? 0x1 : _c_ == 'B' ? -0x1 : '\0';

    if (_set_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set_;

            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            if (
                _logic_is_jump_one_(_id_, _c_, _p_)
            ) {
                _data_._data_[_p_.x + x][_p_.y + y] = malloc(
                    sizeof(_POINT_)
                );

                *_data_._data_
                    [_p_.x + x][_p_.y + y] = _p_;

                _data_._cnt_ += 0x1;
            }
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            if (
                _logic_is_jump_one_(_id_, _c_, _p_)
            ) {
                _data_._data_[_p_.x + x][_p_.y + y] = malloc(
                    sizeof(_POINT_)
                );

                *_data_._data_
                    [_p_.x + x][_p_.y + y] = _p_;

                _data_._cnt_ += 0x1;
            }
        }
    }

    return(_data_);
}



static _MASK_MOVE_
    _mask_move_ = {};

static _MASK_JUMP_
    _mask_jump_ = {};

bool _logic_create_(void) {
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

        _mask_jump_ = _logic_clear_mask_jump_(_mask_jump_);

        _mask_jump_ = _logic_find_jump_(
            _mask_jump_, _old_, _c_
        );

        if (_mask_jump_._cnt_ != 0x0) {
            if (
                _mask_jump_._data_[_new_.x][_new_.y] != NULL
            ) {
                _POINT_* _jump_ = _mask_jump_._data_
                    [_new_.x][_new_.y];

                _POINT_ _j_ = *(_jump_);

                _set_board_char_(
                    _j_.x, _j_.y, '.'
                );

                (void)_update_add_(_j_, '.');

                _game_swap_(
                    (_STEP_){ _old_, _new_ }
                );
            } else {
                _new_ = _old_;
            }
        } else {
            _mask_move_ = _logic_clear_mask_move_(_mask_move_);

            _mask_move_ = _logic_find_move_(
                _mask_move_, _old_, _c_
            );

            if (
                _mask_move_._data_[_new_.x][_new_.y]
            ) {
                _game_swap_(
                    (_STEP_){ _old_, _new_ }
                );
            } else {
                _new_ = _old_;
            }
        }
    } else {
        return(false);
    }

    if (
        _game_q_or_k_(_new_, true)
    )
    {
        (void)_update_add_(
            _new_, _get_board_char_(_new_.x, _new_.y)
        );
    }

    *(_step_) = (_STEP_){
        _old_, _new_
    };

    return(true);
}
