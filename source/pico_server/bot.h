#ifndef _BOT_H_
#define _BOT_H_

#include "framework.h"

#include "game.h"

#include "data.h"

/**
 * @file
 *
 * @brief bot.h
 */

/**
 * @brief a function that implements the choosing one of the possible moves
 *
 * as a result, the function returns the coordinates of cells A and B.
 * a cell A is a cell on which a checker standing. a cell B is a cell on which you need to move a checker.
 */
bool _bot_step_(
    _GAME_* _game_, bool _turn_, _POINT_* _p_old_, _POINT_* _p_new_
);

#endif // _BOT_H_
