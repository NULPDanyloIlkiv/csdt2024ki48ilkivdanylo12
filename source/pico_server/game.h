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

//! structure that store information about the game
struct _sGame_ {
    _BOARD_ _board_; _STEP_ _mem_;

    union {
        struct _sFlag_ {
            bool _turn_ : 1; bool _combo_ : 1;
        } _flag_;

        int8_t _flag_i_;
    };
}; typedef struct _sGame_ _GAME_;



//! make a step on a board
void _game_step_make_(
    _GAME_* _game_, _STEP_ _step_
);



//! make a mortal man queen or king
bool _game_q_or_k_(
    _GAME_* _game_, _POINT_ _point_, bool _s_
);



//! turn to make a step
bool _game_turn_(
    _GAME_* _game_, char _c_, bool _s_
);



//! cancel a update on a board
void _game_update_cancel_(
    _GAME_* _game_, _UPDATE_ _update_
);



//! allocate memory and initialize attributes
bool _game_create_(
    _GAME_* _game_, uint8_t _w_, uint8_t _h_, const char* _data_
);

//! free allocated memory
bool _game_destroy_(
    _GAME_* _game_
);

bool _game_copy_(
    _GAME_* _dest_, const _GAME_* _source_
);

#endif // _GAME_H_
