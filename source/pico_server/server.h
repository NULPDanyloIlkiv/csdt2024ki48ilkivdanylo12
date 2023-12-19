#ifndef _SERVER_H_
#define _SERVER_H_

#include "framework.h"

#include "game.h"

/**
 * @file
 *
 * @brief server.h
 */

/**
 * @brief all these functions are implemented in a "server.c" file and declared as static
 *
 * //! allocate memory and initialize attributes
 * bool _server_request_create_(_GAME_* _game_);
 *
 * //! free allocated memory
 * bool _server_request_destroy_(_GAME_* _game_);
 *
 *
 *
 * //! initialize a new game
 * bool _server_request_init_(_GAME_* _game_);
 *
 *
 *
 * //! turn to make a step
 * bool _server_request_turn_(_GAME_* _game_);
 *
 *
 *
 * //! player select a checker -  save it -> _old_ | _new_
 * bool _server_request_step_make_(_GAME_* _game_);
 *
 *
 *
 * //! execute a bot move and save it -> _old_ | _new_
 * bool _server_request_step_bot_(_GAME_* _game_);
 *
 * //! make a move and check the result of a move
 * bool _server_request_step_take_(_GAME_* _game_);
 *
 *
 *
 * //! update a state of checkers on a board
 * bool _server_request_update_(_GAME_* _game_);
 *
 *
 *
 * //! ping the connection with a client
 * bool _server_request_ping_(_GAME_* _game_);
 *
 * //! restart the connection with a client
 * bool _server_request_restart_(_GAME_* _game_);
 *
 * typedef uint8_t _KEY_;
 *
 * bool _server_update_(
 *     _GAME_* _game_, _KEY_ _key_
 * );
 */

//! a server working in a loop and reply to requests sent by a client
bool _server_loop_(_GAME_* _game_);

#endif // _SERVER_H_
