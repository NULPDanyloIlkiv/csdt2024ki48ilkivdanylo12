#include "serial_port.h"

/**
 * @file
 *
 * @brief serial_port.h
 *
 * this file stores the implementation of functions for server operations
 */

/**
 * @brief library for working with COM port data stream
 * @author Ilkiv Danylo KI-48
 *
 * @version v1.0.0
 * @date 12/12/24
 *
 * @warning for the application to work, it is necessary to specify a global visibility area for some functions of the library <pico/stdio.c>
 */

/**
 *  bool stdio_put_string(
 *     const char *s, int len, bool newline, bool no_cr
 *  );
 *
 *  int stdio_get_until(
 *     char *buf, int len, absolute_time_t until
 *  );
*/

//! this function is implemented in the <pico/stdio.c> library, but it is declared as static
bool stdio_put_string(
    const char*, int, bool, bool
);

//! this function is implemented in the <pico/stdio.c> library, but it is declared as static
int stdio_get_until(
    char*, int, absolute_time_t
);



//! reading bytes from the COM port data stream
size_t _com_recv_(
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_read_ = 0x0;

    char* _data_ = (char*)_buffer_;

    while(_bytes_read_ != _size_ * _cnt_) {
        int8_t _res_ = stdio_get_until(
            _data_ + _bytes_read_, (_size_ * _cnt_) - _bytes_read_, at_the_end_of_time
        );

        if (
            _res_ != -0x1
        ) { _bytes_read_ += _res_; }
    }

    return(_bytes_read_);
}

//! writing bytes to the COM port data stream
size_t _com_send_(
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_write_ = 0x0;

    char* _data_ = (char*)_buffer_;

    (void)stdio_put_string(
        _data_, _bytes_write_ = (_size_ * _cnt_), 0x0, 0x1
    );

    return(_bytes_write_);
}



#include "data.h"

//! reading the key used to identify the data
_KEY_ _com_recv_key_(void) {
    _KEY_ _key_ = _kERROR_;

    (void)_com_recv_(
        &_key_, 0x1, sizeof(_key_)
    );

    return(_key_);
}



//! writing a message in the COM port data stream, consisting of a key and data
size_t _com_send_data_(_KEY_ _key_,
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_write_ = 0x0;

    _bytes_write_ += _com_send_(
        &_key_, 0x1, sizeof(_key_)
    );
    _bytes_write_ += _com_send_(
        _buffer_, _cnt_, _size_
    );

    return(_bytes_write_);
}

//! writing a string-message in the COM port data stream, consisting of a key and data
size_t _com_send_message_(
    const char* _message_
)
{
    _KEY_ _key_ = _kMESSAGE_;

    size_t _bytes_write_ = 0x0;

    _bytes_write_ += _com_send_(
        &_key_, 0x1, sizeof(_key_)
    );



    char _char_[_MSG_SIZE_] = { 0x0 };

    strcpy(_char_, _message_);

    _bytes_write_ += _com_send_(
        _char_, 0x1, sizeof(_char_)
    );

    return(_bytes_write_);
}
