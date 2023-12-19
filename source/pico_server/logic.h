#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "framework.h"

#include "game.h"

#include "data.h"

/**
 * @file
 *
 * @brief logic.h
 */

//! a direction of move | 0x0 - ALL | +0x1 - UP | -0x1 - DOWN
int8_t _logic_set_move_(char _c_);
//! a direction of jump | 0x0 - ALL | +0x1 - UP | -0x1 - DOWN
int8_t _logic_set_jump_(char _c_);

void* _logic_all_(
    void* _data_, _GAME_* _game_, _POINT_ _id_, char _c_, int8_t _set_, void* (_f_)(void*, _GAME_*, _POINT_, char, _POINT_, int8_t, int8_t)
);

typedef struct {
    size_t _w_, _h_; void** _data_; size_t _cnt_;
} _MASK_;

_MASK_ _logic_create_mask_(
    _MASK_ _data_, size_t _size_
);

_MASK_ _logic_destroy_mask_(
    _MASK_ _data_
);



bool _logic_is_move_one_(
    _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_
);

_MASK_ _logic_find_move_all_(
    _MASK_ _data_, _GAME_* _game_, _POINT_ _id_, char _c_
);



bool _logic_is_jump_one_(
    _GAME_* _game_, _POINT_ _id_, char _c_, _POINT_ _p_
);
_MASK_ _logic_find_jump_all_(
    _MASK_ _data_, _GAME_* _game_, _POINT_ _id_, char _c_
);



bool _logic_is_jump_all_(
    _GAME_* _game_, _POINT_ _id_, char _c_
);

bool _logic_is_jump_at_least_all_(
    _GAME_* _game_, bool _turn_
);

#include "game.h"

bool _logic_create_(
    _GAME_* _game_
);

bool _logic_destroy_(
    _GAME_* _game_
);

bool _logic_step_(
    _GAME_* _game_, _POINT_ _old_, _POINT_ _new_, _STEP_* _step_
);

#endif // _LOGIC_H_
