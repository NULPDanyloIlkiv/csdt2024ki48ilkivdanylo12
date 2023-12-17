#include "server.h"

#include "serial_port.h"

#include "game.h"
#include "logic.h"

#include "update.h"

/**
 * @file
 *
 * @brief server.c
 */

//! allocate memory and initialize attributes
bool _server_create_(void) {
    return(
        _logic_create_()
    );
}

//! free allocated memory
bool _server_destroy_(void) {
    return(
        _logic_destroy_()
    );
}



//! initialize a new game
bool _server_init_(void)
{
    _INIT_ _init_ = {
        _get_board_w_(), _get_board_h_()
    };

    (void)_com_send_data_(
        _kINIT_, &_init_, 0x1, sizeof(_init_)
    );

    bool _b_ = 0x0;

    if (
        _game_init_()
    )
    {
        (void)_com_send_data_(
            _kINIT_, _get_board_data_(), _get_board_w_() * _get_board_h_(), sizeof(char)
        );

        _b_ = true;
    } else {
        (void)_com_send_message_(
            "__ERROR__ - Failed to init _game_"
        );

        _b_ = false;
    }

    return(_b_);
}



//! turn to make a step
bool _server_turn_(void) {
    bool _turn_ = _get_flag_() & _fTURN_;

    (void)_com_send_data_(
        _kTURN_, &_turn_, 0x1, sizeof(_turn_)
    );

    return(true);
}



static _POINT_
    _old_ = {},
    _new_ = {};

//! player select a checker -  save it -> _old_
bool _server_step_make_old_(void) {
    (void)_com_recv_(
        &_old_, 0x1, sizeof(_old_)
    );

    if (
        _get_board_char_(_old_.x, _old_.y) == '.'
    )
    {
        (void)_com_send_message_(
            "__ERROR__ - Invalid _data_"
        ); return(0x0);
    }

    (void)_com_send_data_(
        _kSTEP_MAKE_OLD_, &_old_, 0x1, sizeof(_old_)
    );

    return(true);
}

//! player move a checker -  save it -> _new_
bool _server_step_make_new_(void) {
    (void)_com_recv_(
        &_new_, 0x1, sizeof(_new_)
    );

    if (
        !_is_inside_board_(_new_.x, _new_.y)
    )
    {
        (void)_com_send_message_(
            "__ERROR__ - Invalid _data_"
        ); return(0x0);
    }

    (void)_com_send_data_(
        _kSTEP_MAKE_NEW_, &_new_, 0x1, sizeof(_new_)
    );

    return(true);
}

//! execute a bot move and save it -> _old_ | _new_
bool _server_step_bot_(void) {
    return(true);
}



//! make a move and check the result of a move
bool _server_step_take_(void) {
    _STEP_ _step_ = {};

    if (
        _logic_step_(_old_, _new_, &_step_)
    ) {
        /*Code...*/
    } else {
        (void)_com_send_message_(
            "__ERROR__ - Failed to make _step_"
        ); return(0x0);
    }

    (void)_com_send_data_(
        _kSTEP_TAKE_, &_step_, 0x1, sizeof(_step_)
    );

    if (
        !_update_send_()
    ) {
        (void)_com_send_message_(
            "__ERROR__ - Failed to send _update_"
        ); return(0x0);
    }

    (void)_update_clear_();

    return(true);
}



//! ping the connection with a client
bool _server_ping_(void) {
    char _char_[_MSG_SIZE_] = { 0x0 };

    (void)_com_recv_(
        _char_, 0x1, sizeof(_char_)
    );

    /*Code...*/

    (void)_com_send_data_(
        _kMESSAGE_, _char_, 0x1, sizeof(_char_)
    ); return(true);
}

//! restart the connection with a client
bool _server_restart_(void) {
    _KEY_ _key_ = _kRESTART_;

    (void)_com_send_(
        &_key_, 0x1, sizeof(_key_)
    );

    bool _b_ = 0x0;

    return(_b_);
}



static bool _active_ = 0x0;

bool _server_update_(_KEY_ _key_)
{
    bool _state_ = 0x0;

    switch(_key_)
    {

    case(_kRESTART_): {
        _state_ = _server_restart_();
    } break;



    case(_kSTEP_MAKE_OLD_):
        { _state_ = _server_step_make_old_(); } break;
    case(_kSTEP_MAKE_NEW_):
        { _state_ = _server_step_make_new_(); } break;

    case(_kSTEP_BOT_):
        { _state_ = _server_step_bot_(); } break;

    case(_kSTEP_TAKE_):
        { _state_ = _server_step_take_(); } break;



    case(_kINIT_):
        { _state_ = _server_init_(); } break;



    case(_kTURN_):
        { _state_ = _server_turn_(); } break;



    case(_kMESSAGE_):
        { _state_ = _server_ping_(); } break;



    default: {
        char _char_[_MSG_SIZE_] = {};

        sprintf(_char_,
            "__ERROR__ - Invalid KEY : %i\n", _key_
        );

        (void)_com_send_message_(_char_);
    } break;

    }

    if (
        _key_ != _kRESTART_
    ) { _active_ &= _state_; }

    return(_state_);
}

bool _server_loop_(void) {
    _active_ = true;

    _active_ &= _server_create_();



    _KEY_ _key_ = _kERROR_;

    while(
        _key_ = _com_recv_key_(), _server_update_(_key_)
    ) { /*Code... */ }



    _active_ &= _server_destroy_();

    return(_active_);
}
