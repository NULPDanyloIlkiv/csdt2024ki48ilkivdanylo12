#ifndef _SERVER_H_
#define _SERVER_H_

#include "framework.h"

/**
 * @file
 *
 * @brief server.h
 */

//! allocate memory and initialize attributes
bool _server_create_(void);

//! free allocated memory
bool _server_destroy_(void);



//! initialize a new game
bool _server_init_(void);



//! player select a checker -  save it -> _old_
bool _server_step_make_old_(void);

//! player move a checker -  save it -> _new_
bool _server_step_make_new_(void);

//! execute a bot move and save it -> _old_ | _new_
bool _server_step_bot_(void);

//! make a move and check the result of a move
bool _server_step_take_(void);



//! ping the connection with a client
bool _server_ping_(void);



typedef uint8_t _KEY_;

bool _server_update_(_KEY_ _key_);

void _server_loop_(void);

#endif // _SERVER_H_
