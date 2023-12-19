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

bool _bot_step_(
    _GAME_* _game_, bool _turn_, _POINT_* _old_, _POINT_* _new_
);

#endif // _BOT_H_
