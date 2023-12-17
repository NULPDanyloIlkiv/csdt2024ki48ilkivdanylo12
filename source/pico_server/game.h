#ifndef _GAME_H_
#define _GAME_H_

#include "framework.h"

#include "data.h"

/**
 * @file
 *
 * @brief game.h
 */

enum _eFlag_ {
    _fTURN_ = 0b0001, _fCOMBO_ = 0b0010
};

char* _get_board_data_();

char _get_board_char_(
    int8_t x, int8_t y
);

char _set_board_char_(
    int8_t x, int8_t y, char _new_
);

int _get_board_w_(void);

int _get_board_h_(void);



//! get a flag
int8_t _get_flag_(void);

//! set a flag
int8_t _set_flag_(
    int8_t _flag_
);



//! get a mem-step
_STEP_ _get_mem_(void);

//! set a mem-step
_STEP_ _set_mem_(
    _STEP_ _step_
);



bool _is_inside_board_(
    int x, int y
);

//! make a step on a board
void _game_step_(
    _STEP_ _step_
);

//! make a mortal man queen or king
bool _game_q_or_k_(
    _POINT_ _point_, bool _s_
);



//! turn to make a step
bool _game_turn_(
    char _c_, bool _s_
);



//! allocate memory and initialize attributes
bool _game_create_(void);

//! free allocated memory
bool _game_destroy_(void);

//! initialize a new game
bool _game_init_(void);



bool _act_create_(
    uint8_t _w_, uint8_t _h_, const char* _data_
);

bool _act_destroy_(void);

#endif // _GAME_H_
