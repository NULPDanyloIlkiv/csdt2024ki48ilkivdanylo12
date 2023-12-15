#include "logic.h"

/**
 * @file
 *
 * @brief logic.c
 */

//! structure that store information about the game
static struct _sGame_ {
    int8_t _board_w_, _board_h_; char* _board_data_;
} _game_ = { 0x0, NULL };

typedef struct _sGame_ _GAME_;



char* _get_board_data_(void) {
    return(
        _game_._board_data_
    );
}

char _get_board_char_(
    int8_t x, int8_t y
)
{
    return(
        _game_._board_data_[x + (y * _game_._board_w_)]
    );
}

char _set_board_char_(
    int8_t x, int8_t y, char _new_
)
{
    size_t i = x + (
        y * _game_._board_w_
    );

    char _old_ = '.';
    _old_ = _game_._board_data_[i];

    _game_._board_data_[i] = _new_;

    return(_old_);
}

int _get_board_w_(void) {
    return(_game_._board_w_);
}
int _get_board_h_(void) {
    return(_game_._board_h_);
}



bool _is_inside_board_(int x, int y) {
    return(
        x >= 0x0 && x < _game_._board_w_ && y >= 0x0 && y < _game_._board_h_
    );
}



//! swap character in board
void _board_swap_(_STEP_ _step_) {
    char _c_ = '.';

    _POINT_* _p_ = {
        &_step_._old_, &_step_._new_
    };

    for (
        size_t i = 0x0; i < 0x2; i += 0x1
    ) { _c_ = _set_board_char_(_p_[i].x, _p_[i].y, _c_); }
}

//! make a mortal man queen or king
bool _q_or_k_(_POINT_ _point_) {
    char _c_ = _get_board_char_(
        _point_.x, _point_.y
    );

    bool _b_ = (
        _point_.y == ((_c_ == 'W') || (_c_ == 'Q') ? (_c_ = 'Q', _game_._board_h_ - 0x1) : (_c_ == 'B') || (_c_ == 'K') ? (_c_ = 'K', 0x0) : -0x1)
    );

    if (_b_)
        _set_board_char_(
            _point_.x, _point_.y, _c_
        );

    return(_b_);
}



//! allocate memory and initialize attributes
bool _game_create_(void) {
    const uint8_t
        _w_ = _gSIZE_W_,
        _h_ = _gSIZE_H_;

    _game_._board_w_ = _w_, _game_._board_h_ = _h_;

    const size_t
        _size_ = _game_._board_w_ * _game_._board_h_;

    _game_._board_data_ = calloc(
        _size_, sizeof(char)
    );

    return(
        _game_._board_data_ != NULL
    );
}

//! free allocated memory
bool _game_destroy_(void) {
    _game_._board_w_ = _game_._board_h_ = 0x0;

    bool _b_ = 0x0;

    if (
        _game_._board_data_ != NULL
    ) {
        free(
            _game_._board_data_
        ); _game_._board_data_ = NULL;



        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

//! initialize a new game
bool _game_init_(void) {
    bool _b_ = 0x0;

    if (_game_._board_data_ != NULL) {
        const size_t
            _size_ = _game_._board_w_ * _game_._board_h_;

        memcpy(
            _game_._board_data_, _gDATA_, _size_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
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

            free(_data_._data_); _data_._data_ = NULL; _data_._w_ = _data_._w_ = 0x0;
        }
    }

    return(_data_);
}

void _logic_clear_mask_move_(
    _MASK_MOVE_ _data_
)
{
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { _data_._data_[x][y] = NULL; }
}

_MASK_MOVE_ _logic_destroy_mask_move_(
    _MASK_MOVE_ _data_
)
{
    for (
        size_t i = 0x0; i < _data_._w_; i += 0x1
    ) { free(_data_._data_[i]); _data_._data_[i] = NULL; }

    free(_data_._data_); _data_._data_ = NULL; _data_._w_ = _data_._h_ = 0x0;

    return(_data_);
}



void _logic_calc_move_(
    _MASK_MOVE_ _data_, _POINT_ _id_, int8_t x, int8_t y
)
{
    _POINT_ _p_ = {
        _id_.x + x, _id_.y + y
    };

    if (
        _is_inside_board_(_p_.x, _p_.y)
    )
    {
        _data_._data_[_p_.x][_p_.y] = _get_board_char_(
            _p_.x, _p_.y
        ) == '.';
    }
}

_MASK_MOVE_ _logic_find_move_(
    _MASK_MOVE_ _data_, _POINT_ _id_
)
{
    char _c_ = _get_board_char_(
        _id_.x, _id_.y
    );

    int8_t _set_ = (_c_ == 'W') ? 0x1 : _c_ == 'B' ? -0x1 : 0x0;

    if (_set_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set_;

            _logic_calc_move_(
                _data_, _id_, x, y
            );
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _logic_calc_move_(
                _data_, _id_, x, y
            );
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

            free(_data_._data_); _data_._data_ = NULL; _data_._w_ = _data_._h_ = 0x0;
        }
    }

    return(_data_);
}

void _logic_clear_mask_jump_(
    _MASK_JUMP_ _data_
)
{
    for (
        int8_t x = 0x0; x < _data_._w_; x += 0x1
    )
    for (
        int8_t y = 0x0; y < _data_._h_; y += 0x1
    ) { free(_data_._data_[x][y]); _data_._data_[x][y] = NULL; }

}

_MASK_JUMP_ _logic_destroy_mask_jump_(
    _MASK_JUMP_ _data_
)
{
    for (
        int8_t i = 0x0; i < _data_._w_; i += 0x1
    ) { free(_data_._data_[i]); _data_._data_[i] = NULL; }

    free(_data_._data_); _data_._data_ = NULL; _data_._w_ = _data_._h_ = 0x0;
}



void _logic_calc_jump_(
    _MASK_JUMP_ _data_, _POINT_ _id_, int8_t x, int8_t y
)
{
    char _c0_ = _get_board_char_(
        _id_.x, _id_.y
    );

    _POINT_ _p_ = {
        _id_.x + x, _id_.y + y
    };

    if (
        _is_inside_board_(_p_.x, _p_.y)
    )
    {
        char _c1_ = _get_board_char_(
            _p_.x, _p_.y
        );

        if (
            _c1_ != '.' && _c0_ != _c1_ && _c0_ != (
                _c1_ == 'W' ? 'Q' : _c1_ == 'Q' ? 'W' : _c1_ == 'B' ? 'K' : _c1_ == 'K' ? 'B' : '\0'
            )
        )
        {
            if (
                _is_inside_board_(_p_.x + x, _p_.y + y)
            )
            {
                _data_._data_[_p_.x + x][_p_.y + y] = malloc(
                    sizeof(_POINT_)
                );

                *_data_._data_
                    [_p_.x + x][_p_.y + y] = _p_;
            }
        }
    }
}
_MASK_JUMP_ _logic_find_jump_(
    _MASK_JUMP_ _data_, _POINT_ _id_
)
{
    char _c0_ = _get_board_char_(
        _id_.x, _id_.y
    );

    int8_t _set0_ = (_c0_ == 'W') ? 0x1 : _c0_ == 'B' ? -0x1 : '\0';

    if (_set0_ != 0x0) {
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            int8_t y = _set0_;

            _logic_calc_jump_(
                _data_, _id_, x, y
            );
        }
    } else {
        for (int8_t y = -0x1; y < 0x2; y += 0x2)
        for (int8_t x = -0x1; x < 0x2; x += 0x2) {
            _POINT_ _p_ = {
                _id_.x + x, _id_.y + y
            };

            _logic_calc_jump_(
                _data_, _id_, x, y
            );
        }
    }

    return(_data_);
}
