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

int8_t _cClient_::_map_w_(void) const
{ return(m_nMapW); }
int8_t _cClient_::_map_h_(void) const
{ return(m_nMapH); }

std::vector<
    _sObject_
>& _cClient_::_data_(void) {
    return(m_vecObjects);
}

//! the client class constructor
_cClient_::_cClient_(
    const wchar_t* wcPortID
)
{
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

    _game_init_();
#endif // _COM_PORT_
}

//! the client class destructor
_cClient_::~_cClient_(void) {
#ifdef _COM_PORT_
    _com_close_(&m_sp);
#endif // _COM_PORT_
}

#ifdef _COM_PORT_

//! send a message that stores [key + data] indicating the initialization of a new game
void _cClient_::_game_init_(void) {
    _com_send_key_(&m_sp, _kINIT_);

    size_t _bytes_recv_ = 0x0;

    _KEY_ _key_ = _com_recv_key_(&m_sp);
    _bytes_recv_ += sizeof(_key_);

    switch(_key_)
    {
    case(_kINIT_): {
        _INIT_ _init_ = { 0x0 };

        _bytes_recv_ += _com_recv_(
            &m_sp, &_init_, 0x1, sizeof(_init_)
        );



        fwprintf(stderr,
            L"__BYTES_RECV__ : %i | _INIT_ : %d\n", _bytes_recv_, _init_._size_
        );

        m_nMapW = _init_._size_, m_nMapH = _init_._size_;

        fwprintf(stderr,
            L"__BOARD__ : %d x %d\n", m_nMapW, m_nMapH
        );



        _key_ = _com_recv_key_(&m_sp);
        _bytes_recv_ += sizeof(_key_);

        switch(_key_)
        {
        case(_kINIT_): {
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
                }

                fwprintf(stderr,
                    L"%s\n", _board_
                );

                delete[] _board_;
            }
        } break;

        case(_kMESSAGE_): {
            (void)_com_message_(&m_sp);
        } break;

        default: {
            fwprintf(
                stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
            );

            _com_clear_(&m_sp);
        } break;

        }

    } break;



    default: {
        fwprintf(
            stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
        );

        _com_clear_(&m_sp);
    } break;

    }
}



//! send a message that stores [key + data] indicating the execution of a player move
void _cClient_::_step_(_POINT_& _point_) {
    size_t _bytes_recv_ = 0x0;

    _KEY_ _key_ = _com_recv_key_(&m_sp);
    _bytes_recv_ += sizeof(_key_);

    switch(_key_)
    {
    case(_kSTEP_MAKE_OLD_): case(_kSTEP_MAKE_NEW_): {
        _bytes_recv_ += _com_recv_(
            &m_sp, &_point_, 0x1, sizeof(_point_)
        );

        fwprintf(stderr, L"__BYTES_RECV__ : %i %s : %i %i\n",
            _bytes_recv_, _key_ == _kSTEP_MAKE_OLD_ ? "OLD" : "NEW", _point_.x, _point_.y
        );
    } break;

    case(_kMESSAGE_): {
        (void)_com_message_(&m_sp);
    } break;

    default: {
        fwprintf(
            stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
        );

        _com_clear_(&m_sp);
    } break;

    }
}



//! implements the start and finish of the execution of a player move
void _cClient_::_step_make_(
    _KEY_ _key_, _POINT_ _point_
)
{
    size_t _bytes_send_ = 0x0;

    _bytes_send_ += _com_send_data_(&m_sp,
        _key_, &_point_, 0x1, sizeof(_point_)
    );

    fwprintf(stderr, L"__BYTES_SEND__ : %i NEW : %i %i\n",
        _bytes_send_, _point_.x, _point_.y
    );

    _step_(_point_);
}



//! send a message that stores a [key + data] indicating the execution of a bot move
void _cClient_::_step_bot_(void) {
    /*Code...*/
}



//! recv a message that stores a [key + data] indicating a result of a player / bot move | update a map and objects
void _cClient_::_step_take_(void) {
    _com_send_key_(&m_sp, _kSTEP_TAKE_);

    size_t _bytes_recv_ = 0x0;

    _KEY_ _key_ = _com_recv_key_(&m_sp);
    _bytes_recv_ += sizeof(_key_);

    switch(_key_)
    {
    case(_kSTEP_TAKE_): {
        _STEP_ _s_ = {
            { 0x0 }, { 0x0 }
        };

        _bytes_recv_ += _com_recv_(
            &m_sp, &_s_, 0x1, sizeof(_s_)
        );

        fwprintf(stderr, L"__BYTES_RECV__ : %i | OLD | NEW : %d %d -> %d %d\n",
            _bytes_recv_, _s_._old_.x, _s_._old_.y, _s_._new_.x, _s_._new_.y
        );

        for(auto& _o_ : m_vecObjects)
            if (
                _o_.cType != '.' && _s_._old_.x == int(_o_.vOldPos.x) && _s_._old_.y == int(_o_.vOldPos.y)
            )
            {
                _o_.vNewPos = {
                    _s_._new_.x + 0.5f, _s_._new_.y + 0.5f
                }; _o_.vOldPos = _o_.vNewPos; break;
            }



        _key_ = _com_recv_key_(&m_sp);
        _bytes_recv_ += sizeof(_key_);

        switch(_key_)
        {
        case(_kUPDATE_): {
            size_t _cnt_ = 0x0;

            _bytes_recv_ += _com_recv_(
                &m_sp, &_cnt_, 0x1, sizeof(_cnt_)
            );

            for(
                size_t i = 0x0; i < _cnt_; i += 0x1
            )
            {
                _UPDATE_ _update_ = {
                    { -0x1, -0x1 }, 0x0
                };

                _bytes_recv_ += _com_recv_(
                    &m_sp, &_update_, 0x1, sizeof(_update_)
                );

                auto it = std::transform(
                    m_vecObjects.cbegin(), m_vecObjects.cend(), m_vecObjects.begin(), [&](const _sObject_& o)
                    {
                        _sObject_ _o_ = o;

                        if (
                            _update_._id_.x == int(_o_.vNewPos.x) && _update_._id_.y == int(_o_.vNewPos.y)
                        )
                        {
                            if (
                                !(_o_.cType != '.')
                            ) {
                                return(_o_);
                            } else {
                                fwprintf(stderr, L"__BYTES_RECV__ : %i UPDATE : %c %i %i\n",
                                    _bytes_recv_, _o_.cType, int(_o_.vNewPos.x), int(_o_.vNewPos.y)
                                );
                            }

                            _o_.cType = _update_._c_;
                        }

                        return(_o_);
                    }
                );
            }
        } break;

        case(_kMESSAGE_): {
            (void)_com_message_(&m_sp);
        } break;

        default: {
            fwprintf(
                stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
            );

            _com_clear_(&m_sp);
        } break;

        }
    } break;

    case(_kMESSAGE_): {
        (void)_com_message_(&m_sp);
    } break;

    default: {
        fwprintf(
            stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
        );

        _com_clear_(&m_sp);
    } break;

    }
}

#endif _COM_PORT_
