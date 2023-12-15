#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "framework.h"

#include "serial_port.h"
#include "data.h"

#include <vector>

#include <functional>

/**
 * @file
 *
 * @brief client.h
 */

#define _COM_PORT_

//! a structure describing a object the client is working with
struct _sObject_ {
    vec2f
        vOldPos = { 0.f },
        vNewPos = { 0.f };
    char cType = '.';
};

/**
 * @brief the client class
 *
 * this class implements the client
 */
class _cClient_ {

    //! recv a key and handle a data by callback
    bool _handle_key_data_(
        _KEY_ const& _k_, std::function<void()> const& _f_
    );

public:

    /**
     * functions that realize the interaction between a client and a server
     */

#ifdef _COM_PORT_
    //! send a message that stores [key + data] indicating the initialization of a new game
    void _game_init_(void);

    //! send a message that stores [key + data] indicating the execution of a player move
    void _step_make_(
        _KEY_ _key_, _POINT_ _point_
    );

    //! send a message that stores a [key + data] indicating the execution of a bot move
    void _step_bot_(void);

    //! recv a message that stores a [key + data] indicating a result of a player / bot move | update a map and objects
    void _step_take_(void);
#endif // _COM_PORT_

private:

    /**
     * class attributes
     */

#ifdef _COM_PORT_
    _SERIAL_PORT_ m_sp =  { NULL };
#endif // _COM_PORT_

    uint8_t
        m_nMapW = 0x5,
        m_nMapH = 0x5;

    std::vector<_sObject_>
        m_vecObjects;

public:

    /**
     * class constructor / destructor
     */

    _cClient_(
        const wchar_t* wcPortID
    ); ~_cClient_(void);

    int8_t _map_w_(void) const;
    int8_t _map_h_(void) const;

    std::vector<
        _sObject_
    >& _data_(void);
};

#endif // _CLIENT_H_
