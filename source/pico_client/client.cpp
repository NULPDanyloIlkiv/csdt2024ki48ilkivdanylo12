#include "client.h"

#include <cassert>
#include <algorithm>

#define _COM_PORT_

/**
 * @file
 *
 * @brief client.cpp
 *
 * this file stores the implementation of the client class that implements the client
 */

//! #define _LOG_TO_CONSOLE_

#ifdef _LOG_TO_CONSOLE_
static FILE* m_log = stderr;
#else
static FILE* m_log = NULL;
#endif // _LOG_TO_CONSOLE_

uint8_t _cClient_::_map_w_(void) const
{ return(m_nMapW); }
uint8_t _cClient_::_map_h_(void) const
{ return(m_nMapH); }

std::vector<
    _sObject_
> _cClient_::_data_(void) {
    return(m_vecObjects);
}

//! the client class constructor
_cClient_::_cClient_(
    const wchar_t* wcPortID
)
{
#ifndef _LOG_TO_CONSOLE_
    m_log = fopen(
        ".log.txt", "w"
    );
#endif // _LOG_TO_CONSOLE_

    m_sp = { wcPortID };

#ifdef _COM_PORT_
    _com_open_(&m_sp);

    assert(
        m_sp.hPort != INVALID_HANDLE_VALUE
    );

    _com_setup_(
        &m_sp, CBR_115200
    );

    _com_mask_(
        &m_sp, EV_RXCHAR
    );
#endif // _COM_PORT_
}

//! the client class destructor
_cClient_::~_cClient_(void) {
#ifdef _COM_PORT_
    _com_close_(&m_sp);
#endif // _COM_PORT_

#ifndef _LOG_TO_CONSOLE_
    fclose(m_log);
#endif // _LOG_TO_CONSOLE_
}

#ifdef _COM_PORT_

//! //! recv a key and handle a data by callback
bool _cClient_::_handle_key_data_(
    _KEY_ const& _k_, std::function<void()> const& _f_
)
{
    size_t _bytes_recv_ = 0x0;

    _KEY_ _key_ = _com_recv_key_(&m_sp);
    _bytes_recv_ += sizeof(_key_);

    fwprintf(m_log,
        L"__KEY__ : %i.b %i\n", _bytes_recv_, _key_
    );

    bool _b_ = 0x0;

    if (_key_ == _k_) {
        if (
            _f_ != nullptr
        ) {_f_(); }

        _b_ = true;
    } else if (_key_ == _kMESSAGE_) {
        (void)_com_message_(&m_sp); _b_ = false;
    } else {
        fwprintf(
            m_log, L"__ERROR__ - Invalid KEY : %i\n", _key_
        );

        _com_clear_(&m_sp);

        _b_ = false;
    }

    return(_b_);
}



//! send a message that stores [key + data] indicating the initialization of a new game
void _cClient_::_game_init_(void) {
    _com_send_key_(
        &m_sp, _kINIT_
    );



    (void)_handle_key_data_(_kINIT_, [&](void)
    {
        _INIT_ _init_ = {};

        size_t _bytes_recv_ = 0x0;

        _bytes_recv_ += _com_recv_(
            &m_sp, &_init_, 0x1, sizeof(_init_)
        );



        fwprintf(m_log,
            L"__BYTES_RECV__ : %i.b | _INIT_ : %d %d\n", _bytes_recv_, _init_._size_w_, _init_._size_h_
        );

        m_nMapW = _init_._size_w_, m_nMapH = _init_._size_h_;

        fwprintf(m_log,
            L"__BOARD__ : %d x %d\n", m_nMapW, m_nMapH
        );



        (void)_handle_key_data_(_kINIT_, [&](void)
        {
            if (
                !m_vecObjects.empty()
            ) { m_vecObjects.clear(); }

            for (
                size_t j = 0x0; j < m_nMapH; j += 0x1
            )
            {
                const size_t _cnt_ = m_nMapW;

                char* _board_ = new char[_cnt_];

                _bytes_recv_ += _com_recv_(
                    &m_sp, _board_, _cnt_, sizeof(char)
                );

                for (
                    size_t i = 0x0; i < m_nMapW; i += 0x1
                )
                {
                    char _char_ = _board_[i];

                    if (_char_ != '.') {
                        vec2f v = { i + 0.5f, j + 0.5f };
                        _sObject_ o = {
                            v, v, _char_
                        };

                        m_vecObjects.
                            push_back(o);
                    }

                    fwprintf(m_log,
                        L"%c%c", _board_[i], i != m_nMapW - 0x1 ? '\0' : '\n'
                    );
                }

                delete[] _board_;
            }
        }
        );
    }
    );
}



//! send a message that stores [key + data] indicating the execution of a player move
void _cClient_::_step_make_(
    _STEP_ _step_
)
{
    size_t _bytes_send_ = 0x0;

    _bytes_send_ += _com_send_data_(&m_sp,
        _kSTEP_MAKE_, &_step_, 0x1, sizeof(_step_)
    );

    fwprintf(m_log, L"__BYTES_SEND__ : %i.b OLD : %i %i | NEW : %i %i\n",
        _bytes_send_, _step_._old_.x, _step_._old_.y, _step_._new_.x, _step_._new_.y
    );



    (void)_handle_key_data_(_kSTEP_MAKE_, [&](void)
        {
            size_t _bytes_recv_ = 0x0;

            _bytes_recv_ += _com_recv_(
                &m_sp, &_step_, 0x1, sizeof(_step_)
            );

            fwprintf(m_log, L"__BYTES_RECV__ : %i.b OLD : %i %i | NEW : %i %i\n",
                _bytes_recv_, _step_._old_.x, _step_._old_.y, _step_._new_.x, _step_._new_.y
            );
        }
    );
}



//! send a message that stores a [key + data] indicating the execution of a bot move
void _cClient_::_step_bot_(void) {
    _com_send_key_(
        &m_sp, _kSTEP_BOT_
    );

    (void)_handle_key_data_(_kSTEP_BOT_, [&](void)
        {
            _STEP_ _s_ = {};

            size_t _bytes_recv_ = 0x0;

            _bytes_recv_ += _com_recv_(
                &m_sp, &_s_, 0x1, sizeof(_s_)
            );

            fwprintf(m_log, L"__BYTES_RECV__ : %i.b | BOT : OLD | NEW : %d %d -> %d %d\n",
                _bytes_recv_, _s_._old_.x, _s_._old_.y, _s_._new_.x, _s_._new_.y
            );
        }
    );
}



//! recv a message that stores a [key + data] indicating a result of a player / bot move
void _cClient_::_step_take_(void) {
    _com_send_key_(
        &m_sp, _kSTEP_TAKE_
    );



    (void)_handle_key_data_(_kSTEP_TAKE_, [&](void)
    {
        _STEP_ _s_ = {};

        size_t _bytes_recv_ = 0x0;

        _bytes_recv_ += _com_recv_(
            &m_sp, &_s_, 0x1, sizeof(_s_)
        );

        fwprintf(m_log, L"__BYTES_RECV__ : %i.b | OLD | NEW : %d %d -> %d %d\n",
            _bytes_recv_, _s_._old_.x, _s_._old_.y, _s_._new_.x, _s_._new_.y
        );

        /*
        for(auto& _o_ : m_vecObjects)
            if (
                _o_.cType != '.' && _s_._old_.x == int(_o_.vOldPos.x) && _s_._old_.y == int(_o_.vOldPos.y)
            )
            {
                _o_.vNewPos = {
                    _s_._new_.x + 0.5f, _s_._new_.y + 0.5f
                }; _o_.vOldPos = _o_.vNewPos; break;
            }
        */
    }
    );
}



//! update a map and objects
void _cClient_::_update_(void) {
    _com_send_key_(
        &m_sp, _kUPDATE_
    );



    (void)_handle_key_data_(_kUPDATE_, [&](void)
    {
        size_t _cnt_ = 0x0;

        size_t _bytes_recv_ = 0x0;

        _bytes_recv_ += _com_recv_(
            &m_sp, &_cnt_, 0x1, sizeof(_cnt_)
        );

        for(
            size_t i = 0x0; i < _cnt_; i += 0x1
        )
        {
            _UPDATE_ _update_ = {};

            _bytes_recv_ += _com_recv_(
                &m_sp, &_update_, 0x1, sizeof(_update_)
            );

            (void)std::transform(
                m_vecObjects.cbegin(), m_vecObjects.cend(), m_vecObjects.begin(), [&](const _sObject_& o)
                {
                    _sObject_ _o_ = o;

                    if (
                        _update_._old_._c_ == _o_.cType && _update_._old_._id_.x == int(_o_.vNewPos.x) && _update_._old_._id_.y == int(_o_.vNewPos.y)
                    )
                    {
                        if (
                            !(_o_.cType != '.')
                        ) {
                            return(_o_);
                        } else {
                            fwprintf(m_log, L"__BYTES_RECV__ : %i.b UPDATE : %c %i %i\n",
                                _bytes_recv_, _o_.cType, int(_o_.vNewPos.x), int(_o_.vNewPos.y)
                            );
                        }

                        _o_.vOldPos = _o_.vNewPos;

                        _o_.vNewPos = {
                            _update_._new_._id_.x + 0.5f, _update_._new_._id_.y + 0.5f
                        };

                        _o_.cType = _update_._new_._c_;
                    }

                    return(_o_);
                }
            );
        }
    }
    );
}



void _cClient_::_game_restart_(void) {
    _com_send_key_(
        &m_sp, _kRESTART_
    );

    (void)_handle_key_data_(_kRESTART_, [this](void)
        { fwprintf(m_log, L"__RESTART__\n"); }
    );

    _game_init_();
}

#endif _COM_PORT_
