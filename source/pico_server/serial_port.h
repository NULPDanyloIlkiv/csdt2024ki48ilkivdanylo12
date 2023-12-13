#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include "framework.h"

/**
 * @file
 *
 * @brief serial_port.h
 */

//! reading bytes from the COM port data stream
size_t _com_recv_(
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! writing bytes to the COM port data stream
size_t _com_send_(
    void* _buffer_, size_t _cnt_, size_t _size_
);



typedef uint8_t _KEY_;

//! reading the key used to identify the data
_KEY_ _com_recv_key_(void);



//! writing a message in the COM port data stream, consisting of a key and data
size_t _com_send_data_(_KEY_ _key_,
    void* _buffer_, size_t _cnt_, size_t _size_
);

//! writing a string-message in the COM port data stream, consisting of a key and data
size_t _com_send_message_(
    const char* _message_
);

#endif // _SERIAL_PORT_H_
