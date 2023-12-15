#include "server.h"

#include "serial_port.h"

#include "logic.h"
#include "update.h"

/**
 * @file
 *
 * @brief server.c
 */

static _MASK_MOVE_ _mask_move_ = { 0x0, 0x0, NULL };
static _MASK_JUMP_ _mask_jump_ = { 0x0, 0x0, NULL };

//! allocate memory and initialize attributes
bool _server_create_(void) {
    bool _b_ = 0x0;

    if (
        _game_create_()
    )
    {
        _POINT_ _point_ = {
            _get_board_w_(), _get_board_h_()
        };

        _mask_move_._w_ = _point_.x;
        _mask_move_._h_ = _point_.y;

        _mask_move_ = _logic_create_mask_move_(
            _mask_move_
        );

        _mask_jump_._w_ = _point_.x;
        _mask_jump_._h_ = _point_.y;

        _mask_jump_ = _logic_create_mask_jump_(
            _mask_jump_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
}

//! free allocated memory
bool _server_destroy_(void) {
    bool _b_ = 0x0;

    if (
        _game_destroy_()
    )
    {
        _mask_move_ = _logic_destroy_mask_move_(
            _mask_move_
        );
        _mask_jump_ = _logic_destroy_mask_jump_(
            _mask_jump_
        );

        _b_ = true;
    } else {
        _b_ = false;
    }

    return(_b_);
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
            "ERROR - Failed to send _game_._board_data_"
        );

        _b_ = false;
    }

    return(_b_);
}



static _POINT_
    _old_ = { 0x0 },
    _new_ = { 0x0 };

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
            "ERROR - Invalid DATA"
        ); return(false);
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
            "ERROR - Invalid DATA"
        ); return(false);
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
    if (
        _mask_move_._data_ != NULL && _mask_jump_._data_ != NULL
    )
    {
        _logic_clear_mask_move_(_mask_move_);
        _mask_move_ = _logic_find_move_(
            _mask_move_, _old_
        );

        _logic_clear_mask_jump_(_mask_jump_);
        _mask_jump_ = _logic_find_jump_(
            _mask_jump_, _old_
        );

        if (
            _mask_jump_._data_[_new_.x][_new_.y] != NULL
        ) {
            _POINT_* _jump_ = _mask_jump_._data_
                [_new_.x][_new_.y];

            _POINT_ _j_ = *(_jump_);



            _set_board_char_(
                _j_.x, _j_.y, '.'
            );

            (void)_update_add_(_j_, '.');



            _board_swap_(
                (_STEP_){ _old_, _new_ }
            );
        } else if (
            _mask_move_._data_[_new_.x][_new_.y] != NULL
        ) {
            _board_swap_(
                (_STEP_){ _old_, _new_ }
            );
        } else {
            _new_ = _old_;
        }
    } else {
        (void)_com_send_message_(
            "ERROR - Failed to send _update_._board_data_"
        ); return(false);
    }

    if (
        _q_or_k_(_new_)
    )
    {
        (void)_update_add_(
            _new_, _get_board_char_(_new_.x, _new_.y)
        );
    }

    _STEP_ _step_ = { _old_, _new_ };

    (void)_com_send_data_(
        _kSTEP_TAKE_, &_step_, 0x1, sizeof(_step_)
    );



    if (
        !_update_send_()
    ) {
        (void)_com_send_message_(
            "ERROR - Failed to send _update_"
        ); return(false);
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



static bool _active_ = true;

bool _server_update_(_KEY_ _key_)
{
    bool _state_ = 0x0;

    switch(_key_)
    {

    case(_kRESTART_): {
        _state_ = 0x0;
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
