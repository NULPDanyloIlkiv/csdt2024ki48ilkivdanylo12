#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include "framework.h"

/**
 * @file
 *
 * @brief serial_port.h
 */

//! a structure, which stores everything necessary for working with the data stream of COM port
struct _sSerialPort_ {
    const wchar_t*
        wcPortID = NULL;

    HANDLE hPort = NULL;

    DCB dcbSP = { 0x0 };
    COMMTIMEOUTS
        cmtST = { 0x0 };
}; typedef _sSerialPort_ _SERIAL_PORT_;



//! opening the COM port descriptor
DWORD _com_open_(_SERIAL_PORT_* _port_);

//! closing the COM port descriptor
DWORD _com_close_(_SERIAL_PORT_* _port_);

//! setting the parameters of the COM port
DWORD _com_setup_(_SERIAL_PORT_* _port_,
    DWORD _rate_ = CBR_115200
);

//! setting the COM port mask, which indicates the set of events that should be monitored for the communication device
DWORD _com_mask_(_SERIAL_PORT_* _port_,
    DWORD dwEvtMask = EV_RXCHAR
);



//! reading bytes from the COM port data stream
size_t _com_recv_(_SERIAL_PORT_* _port_,
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! writing bytes to the COM port data stream
size_t _com_send_(_SERIAL_PORT_* _port_,
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! clearing bytes from the COM port data stream
bool _com_clear_(_SERIAL_PORT_* _port_);

#include "data.h"

//! reading the key used to identify the data
_KEY_ _com_recv_key_(
    _SERIAL_PORT_* _port_
);

//! writing the key used to identify the data
void _com_send_key_(
    _SERIAL_PORT_* _port_, _KEY_ _key_
);



//! writing a message in the COM port data stream, consisting of a key and data
size_t _com_send_data_(_SERIAL_PORT_* _port_,
    _KEY_ _key_, void* _buffer_, size_t _cnt_, size_t _size_
);

//! reading a string-message from the COM port data stream, consisting of a key and data
size_t _com_message_(
    _SERIAL_PORT_* _port_
);

#endif // _SERIAL_PORT_H_
