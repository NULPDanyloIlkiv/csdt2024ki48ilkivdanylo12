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

    fwprintf(stderr,
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
            stderr, L"__ERROR__ - Invalid KEY : %i\n", _key_
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
        _INIT_ _init_ = { 0x0, 0x0 };

        size_t _bytes_recv_ = 0x0;

        _bytes_recv_ += _com_recv_(
            &m_sp, &_init_, 0x1, sizeof(_init_)
        );



        fwprintf(stderr,
            L"__BYTES_RECV__ : %i.b | _INIT_ : %d %d\n", _bytes_recv_, _init_._size_w_, _init_._size_h_
        );

        m_nMapW = _init_._size_w_, m_nMapH = _init_._size_h_;

        fwprintf(stderr,
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

                    fwprintf(stderr,
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
    _KEY_ _key_, _POINT_ _point_
)
{
    size_t _bytes_send_ = 0x0;

    _bytes_send_ += _com_send_data_(&m_sp,
        _key_, &_point_, 0x1, sizeof(_point_)
    );

    fwprintf(stderr, L"__BYTES_SEND__ : %i.b NEW : %i %i\n",
        _bytes_send_, _point_.x, _point_.y
    );



    (void)_handle_key_data_(_key_, [&](void)
        {
            size_t _bytes_recv_ = 0x0;

            _bytes_recv_ += _com_recv_(
                &m_sp, &_point_, 0x1, sizeof(_point_)
            );

            fwprintf(stderr, L"__BYTES_RECV__ : %i.b %s : %i %i\n",
                _bytes_recv_, _key_ == _kSTEP_MAKE_OLD_ ? "OLD" : "NEW", _point_.x, _point_.y
            );
        }
    );
}



//! send a message that stores a [key + data] indicating the execution of a bot move
void _cClient_::_step_bot_(void) {
    /*Code...*/
}



//! recv a message that stores a [key + data] indicating a result of a player / bot move | update a map and objects
void _cClient_::_step_take_(void) {
    _com_send_key_(
        &m_sp, _kSTEP_TAKE_
    );



    (void)_handle_key_data_(_kSTEP_TAKE_, [&](void)
    {
        _STEP_ _s_ = {
            { 0x0 }, { 0x0 }
        };

        size_t _bytes_recv_ = 0x0;

        _bytes_recv_ += _com_recv_(
            &m_sp, &_s_, 0x1, sizeof(_s_)
        );

        fwprintf(stderr, L"__BYTES_RECV__ : %i.b | OLD | NEW : %d %d -> %d %d\n",
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



        (void)_handle_key_data_(_kUPDATE_, [&](void)
        {
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
                                fwprintf(stderr, L"__BYTES_RECV__ : %i.b UPDATE : %c %i %i\n",
                                    _bytes_recv_, _o_.cType, int(_o_.vNewPos.x), int(_o_.vNewPos.y)
                                );
                            }

                            _o_.cType = _update_._c_;
                        }

                        return(_o_);
                    }
                );
            }
        }
        );
    }
    );
}

void _cClient_::_game_restart_(void) {
    _com_send_key_(
        &m_sp, _kRESTART_
    );

    (void)_handle_key_data_(_kRESTART_, [this](void)
        { fwprintf(stderr, L"__RESTART__\n"); }
    );

    _game_init_();
}

#endif _COM_PORT_
