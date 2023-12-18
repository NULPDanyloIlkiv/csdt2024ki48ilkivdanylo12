#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include "framework.h"

#include "data.h"

//! #define _COM_UART_

/**
 * @file
 *
 * @brief serial_port.h
 */

#ifdef _COM_UART_

//! UART interface initialization
uint16_t _uart_init_(uint16_t _rate_);

#endif // _COM_UART_

//! reading bytes from the COM port data stream
size_t _com_recv_(
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! writing bytes to the COM port data stream
size_t _com_send_(
    void* _buffer_, size_t _cnt_, size_t _size_
);



typedef uint8_t _KEY_;

//! reading a key used to identify a data
_KEY_ _com_recv_key_(void);

//! writing a key used to identify a data
void _com_send_key_(_KEY_ _key_);



//! writing a message in the COM port data stream, consisting of a key and data
size_t _com_send_data_(_KEY_ _key_,
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! writing a string-message in the COM port data stream, consisting of a key and data
size_t _com_send_message_(
    const char* _message_
);

#endif // _SERIAL_PORT_H_
