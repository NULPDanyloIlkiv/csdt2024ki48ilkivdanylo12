#ifndef _SERVER_H_
#define _SERVER_H_

#include "framework.h"

/**
 * @file
 *
 * @brief server.h
 */

/**
 * @brief all these functions are implemented in a "server.c" file and declared as static
 *
 * //! allocate memory and initialize attributes
 * bool _server_request_create_(void);
 *
 * //! free allocated memory
 * bool _server_request_destroy_(void);
 *
 *
 *
 * //! initialize a new game
 * bool _server_request_init_(void);
 *
 *
 *
 * //! turn to make a step
 * bool _server_request_turn_(void);
 *
 *
 *
 * //! player select a checker -  save it -> _old_
 * bool _server_request_step_make_old_(void);
 *
 * //! player move a checker -  save it -> _new_
 * bool _server_request_step_make_new_(void);
 *
 * //! execute a bot move and save it -> _old_ | _new_
 * bool _server_request_step_bot_(void);
 *
 * //! make a move and check the result of a move
 * bool _server_request_step_take_(void);
 *
 *
 *
 * //! ping the connection with a client
 * bool _server_request_ping_(void);
 *
 * //! restart the connection with a client
 * bool _server_request_restart_(void);
 *
 * typedef uint8_t _KEY_;
 *
 * bool _server_update_(
 *     _KEY_ _key_
 * );
 */

//! a server working in a loop and reply to requests sent by a client
bool _server_loop_(void);

#endif // _SERVER_H_
