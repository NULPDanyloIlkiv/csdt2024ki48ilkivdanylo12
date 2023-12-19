#include "server.h"

#include "serial_port.h"

#include "game.h"
#include "logic.h"

#include "bot.h"

#include "update.h"

/**
 * @file
 *
 * @brief server.c
 */

//! allocate memory and initialize attributes
static bool _server_create_(_GAME_* _game_) {
    return(
        _logic_create_(_game_)
    );
}

//! free allocated memory
static bool _server_destroy_(_GAME_* _game_) {
    return(
        _logic_destroy_(_game_)
    );
}



//! initialize a new game
static bool _server_request_init_(_GAME_* _game_)
{
    _BOARD_ _board_ = _game_->_board_;

    const uint8_t
        _w_ = _board_._w_,
        _h_ = _board_._h_;



    _INIT_ _init_ = {
        _w_, _h_
    };

    (void)_com_send_data_(
        _kINIT_, &_init_, 0x1, sizeof(_init_)
    );



    bool _b_ = 0x0;

    if (
        _game_ != NULL
    )
    {
        const char* _data_ = _board_._data_;

        (void)_com_send_data_(
            _kINIT_, (char*)_data_, _w_ * _h_, sizeof(char)
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
static bool _server_request_turn_(_GAME_* _game_) {
    bool _turn_ = _game_->_flag_i_ & _fTURN_;

    (void)_com_send_data_(
        _kTURN_, &_turn_, 0x1, sizeof(_turn_)
    );

    return(true);
}



static _POINT_
    _old_ = { -0x1, -0x1 },
    _new_ = { -0x1, -0x1 };

//! player select a checker -  save it -> _old_ | _new_
static bool _server_request_step_make_(_GAME_* _game_) {
    _STEP_ _step_ = {};

    (void)_com_recv_(
        &_step_, 0x1, sizeof(_step_)
    );

    _BOARD_ _board_ = _game_->_board_;

    if (
        !(_is_inside_board_(_board_, _step_._old_.x, _step_._old_.y) && _is_inside_board_(_board_, _step_._new_.x, _step_._new_.y))
    ) {
        (void)_com_send_message_(
            "__ERROR__ - Invalid _data_"
        ); return(0x0);
    }

    _old_ = _step_._old_,
    _new_ = _step_._new_;

    (void)_com_send_data_(
        _kSTEP_MAKE_, &_step_, 0x1, sizeof(_step_)
    );

    return(true);
}

//! execute a bot move and save it -> _old_ | _new_
static bool _server_request_step_bot_(_GAME_* _game_) {
    if (
        !_bot_step_(_game_, _game_->_flag_i_ & _fTURN_, &_old_, &_new_)
    ) {
        (void)_com_send_message_(
            "__ERROR__ - Invalid _data_"
        ); return(0x0);
    }

    _STEP_ _step_ = (_STEP_) {
        _old_, _new_
    };

    (void)_com_send_data_(
        _kSTEP_BOT_, &_step_, 0x1, sizeof(_step_)
    );

    return(true);
}



//! make a move and check the result of a move
static bool _server_request_step_take_(_GAME_* _game_) {
    _STEP_ _step_ = {};

    if (
        _logic_step_(_game_, _old_, _new_, &_step_)
    ) {
        /*Code...*/
    } else {
        (void)_com_send_message_(
            "__ERROR__ - Failed to make _step_"
        ); return(0x0);
    }

    _old_ = (_POINT_){ -0x1, -0x1 },
    _new_ = (_POINT_){ -0x1, -0x1 };

    (void)_com_send_data_(
        _kSTEP_TAKE_, &_step_, 0x1, sizeof(_step_)
    );

    return(true);
}



//! update a state of checkers on a board
bool _server_request_update_(_GAME_* _game_) {
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
static bool _server_request_ping_(_GAME_* _game_) {
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
static bool _server_request_restart_(_GAME_* _game_) {
    _KEY_ _key_ = _kRESTART_;

    (void)_com_send_(
        &_key_, 0x1, sizeof(_key_)
    );

    bool _b_ = 0x0;

    return(_b_);
}



static bool _active_ = 0x0;

static bool _server_update_(
    _GAME_* _game_, _KEY_ _key_
) {
    bool _state_ = 0x0;

    switch(_key_)
    {

    case(_kRESTART_): {
        _state_ = _server_request_restart_(_game_);
    } break;



    case(_kSTEP_MAKE_):
        { _state_ = _server_request_step_make_(_game_); } break;

    case(_kSTEP_BOT_):
        { _state_ = _server_request_step_bot_(_game_); } break;

    case(_kSTEP_TAKE_):
        { _state_ = _server_request_step_take_(_game_); } break;



    case(_kUPDATE_):
        { _state_ = _server_request_update_(_game_); } break;



    case(_kINIT_):
        { _state_ = _server_request_init_(_game_); } break;



    case(_kTURN_):
        { _state_ = _server_request_turn_(_game_); } break;



    case(_kMESSAGE_):
        { _state_ = _server_request_ping_(_game_); } break;



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

//!#define _COM_UART_

#ifdef _COM_UART_
#define _BAUD_RATE_ 115200
#endif // _COM_UART_

//! a server working in a loop and reply to requests sent by a client
bool _server_loop_(_GAME_* _game_) {
#ifdef _COM_UART_
    (void)_uart_init_(_BAUD_RATE_);
#endif // _COM_UART_

    _active_ = true;

    _active_ &= _server_create_(_game_);



    _KEY_ _key_ = _kERROR_;

    while(
        _key_ = _com_recv_key_(), _server_update_(_game_, _key_)
    ) { /*Code... */ }



    _active_ &= _server_destroy_(_game_);

    return(_active_);
}
