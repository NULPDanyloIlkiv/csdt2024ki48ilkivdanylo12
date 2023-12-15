#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "framework.h"

#include "data.h"

/**
 * @file
 *
 * @brief logic.h
 */

char* _get_board_data_();

char _get_board_char_(
    int8_t x, int8_t y
);

char _set_board_char_(
    int8_t x, int8_t y, char _new_
);

int _get_board_w_(void);

int _get_board_h_(void);



bool _is_inside_board_(int x, int y);



//! swap character in board
void _board_swap_(_STEP_ _step_);

//! make a mortal man queen or king
bool _q_or_k_(_POINT_ _point_);



//! allocate memory and initialize attributes
bool _game_create_(void);

//! free allocated memory
bool _game_destroy_(void);

//! initialize a new game
bool _game_init_(void);



typedef struct {
    size_t _w_, _h_; bool** _data_;
} _MASK_MOVE_;

_MASK_MOVE_ _logic_create_mask_move_(
    _MASK_MOVE_ _data_
);

void _logic_clear_mask_move_(
    _MASK_MOVE_ _data_
);

_MASK_MOVE_ _logic_destroy_mask_move_(
    _MASK_MOVE_ _data_
);



void _logic_calc_move_(
    _MASK_MOVE_ _data_, _POINT_ _id_, int8_t x, int8_t y
);

_MASK_MOVE_ _logic_find_move_(
    _MASK_MOVE_ _data_, _POINT_ _id_
);



typedef struct {
    size_t _w_, _h_; _POINT_*** _data_;
} _MASK_JUMP_;

_MASK_JUMP_ _logic_create_mask_jump_(
    _MASK_JUMP_ _data_
);

void _logic_clear_mask_jump_(
    _MASK_JUMP_ _data_
);

_MASK_JUMP_ _logic_destroy_mask_jump_(
    _MASK_JUMP_ _data_
);



void _logic_calc_jump_(
    _MASK_JUMP_ _data_, _POINT_ _id_, int8_t x, int8_t y
);
_MASK_JUMP_ _logic_find_jump_(
    _MASK_JUMP_ _data_, _POINT_ _id_
);

#endif // _LOGIC_H_
