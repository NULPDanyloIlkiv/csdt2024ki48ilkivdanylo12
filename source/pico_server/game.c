#include "game.h"

/**
 * @file
 *
 * @brief game.c
 */

//! structure that store information about the game
static struct _sGame_ {
    int8_t _board_w_, _board_h_; char* _board_data_;

    union {
        struct _sFlag_ { bool _turn_ : 1; } _flag_; int8_t _flag_i_;
    };
} _game_ = {}, _act_ = {};

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



bool _is_inside_board_(
    int x, int y
) {
    return(
        x >= 0x0 && x < _game_._board_w_ && y >= 0x0 && y < _game_._board_h_
    );
}



//! swap character in board
void _game_swap_(
    _STEP_ _step_
) {
    char _c_ = '.';

    _POINT_* _p_ = {
        &_step_._old_, &_step_._new_
    };

    for (
        size_t i = 0x0; i < 0x2; i += 0x1
    ) { _c_ = _set_board_char_(_p_[i].x, _p_[i].y, _c_); }
}

//! make a mortal man queen or king
bool _game_q_or_k_(
    _POINT_ _point_, bool _s_
) {
    char _c_ = _get_board_char_(
        _point_.x, _point_.y
    );

    bool _b_ = (
        _point_.y == ((_c_ == 'W') || (_c_ == 'Q') ? (_c_ = 'Q', _game_._board_h_ - 0x1) : (_c_ == 'B') || (_c_ == 'K') ? (_c_ = 'K', 0x0) : -0x1)
    );

    if (_s_ && _b_)
        _set_board_char_(
            _point_.x, _point_.y, _c_
        );

    return(_b_);
}

//! turn to make a step
bool _game_turn_(
    char _c_, bool _s_
) {
    bool _b_ = !(_game_._flag_._turn_) ? (_c_ == 'W' || _c_ == 'Q') : (_c_ == 'B' || _c_ == 'K');

    if (_s_ && _b_) {
        _game_._flag_._turn_ = !_game_._flag_._turn_;
    }

    return(_b_);
}



//! allocate memory and initialize attributes
bool _game_create_(void) {
    _game_._flag_i_ = _act_._flag_i_;

    const uint8_t
        _w_ = _act_._board_w_,
        _h_ = _act_._board_h_;

    _game_._board_w_ = _w_, _game_._board_h_ = _h_;

    const size_t
        _size_ = _game_._board_w_ * _game_._board_h_;

    _game_._board_data_ = calloc(
        _size_, sizeof(char)
    );

    memset(
        _game_._board_data_, '.', _size_
    );



    return(
        _game_._board_data_ != NULL
    );
}

//! free allocated memory
bool _game_destroy_(void) {
    _game_._flag_i_ = _game_._board_w_ = _game_._board_h_ = 0x0;

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

    if (
        _game_._board_data_ != NULL && _act_._board_data_ != NULL
    )
    {
        const size_t
            _size_ = _act_._board_w_ * _act_._board_h_;

        memcpy(
            _game_._board_data_, _act_._board_data_, _size_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}



bool _act_create_(
    uint8_t _w_, uint8_t _h_, const char* _data_
)
{
    _act_._board_w_ = _w_, _act_._board_h_ = _h_;

    const size_t
        _size_ = _act_._board_w_ * _act_._board_h_;

    _act_._board_data_ = calloc(
        _size_, sizeof(char)
    );

    memcpy(
        _act_._board_data_, _data_, _size_
    );



    return(
        _act_._board_data_ != NULL
    );
}

bool _act_destroy_(void) {
    _act_._board_w_ = _act_._board_h_ = 0x0;

    bool _b_ = 0x0;

    if (
        _act_._board_data_ != NULL
    ) {
        free(
            _act_._board_data_
        ); _act_._board_data_ = NULL;

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}
