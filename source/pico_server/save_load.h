#ifndef _SAVE_LOAD_H_
#define _SAVE_LOAD_H_

#include "framework.h"

#include "game.h"

bool _game_save_(
	_GAME_* _game_, const char* _file_
);

bool _game_load_(
	_GAME_* _game_, const char* _file_
);

#endif // _SAVE_LOAD_H_
