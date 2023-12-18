#include "serial_port.h"

//! #define _COM_UART_

/**
 * @file
 *
 * @brief serial_port.c
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
 * @brief <pico/stdio.c>
 *
 * bool stdio_put_string(
 *    const char *s, int len, bool newline, bool no_cr
 * );
 *
 * int stdio_get_until(
 *    char *buf, int len, absolute_time_t until
 * );
*/

//! this function is implemented in the <stdio.c> library, but it is declared as static
bool stdio_put_string(
    const char*, int, bool, bool
);

//! this function is implemented in the <stdio.c> library, but it is declared as static
int stdio_get_until(
    char*, int, absolute_time_t
);



#ifdef _COM_UART_

#include "hardware/uart.h"

#define _UART_ID_ uart0

#define _UART_TX_PIN_ 0x0
#define _UART_RX_PIN_ 0x1

/**
 * @brief <uart.h>
 *
 * void uart_write_blocking(
 *     uart_inst_t *uart, const uint8_t *src, size_t len
 * )
 *
 * void uart_read_blocking(
 *     uart_inst_t *uart, uint8_t *dst, size_t len
 * )
*/

#endif // _COM_UART_



#ifdef _COM_UART_

//! UART interface initialization
uint16_t _uart_init_(uint16_t _rate_) {
    _rate_ = uart_init(_UART_ID_, _rate_);

    gpio_set_function(_UART_TX_PIN_, GPIO_FUNC_UART);
    gpio_set_function(_UART_RX_PIN_, GPIO_FUNC_UART);

    return(_rate_);
}

#endif // _COM_UART_



//! reading bytes from the COM port data stream
size_t _com_recv_(
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_read_ = 0x0;

    uint8_t* _data_ = (uint8_t*)_buffer_;

#ifdef _COM_UART_
    uart_read_blocking(
        _UART_ID_, _data_, _bytes_read_ = (_size_ * _cnt_)
    );
#else
    while(_bytes_read_ != _size_ * _cnt_) {
        int8_t _res_ = stdio_get_until(
            _data_ + _bytes_read_, (_size_ * _cnt_) - _bytes_read_, at_the_end_of_time
        );

        if (
            _res_ != -0x1
        ) { _bytes_read_ += _res_; }
    }
#endif // _COM_UART_

    return(_bytes_read_);
}

//! writing bytes to the COM port data stream
size_t _com_send_(
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_write_ = 0x0;

    uint8_t* _data_ = (uint8_t*)_buffer_;

#ifdef _COM_UART_
    uart_write_blocking(
        _UART_ID_, _data_, _bytes_write_ = (_size_ * _cnt_)
    );
#else
    (void)stdio_put_string(
        _data_, _bytes_write_ = (_size_ * _cnt_), 0x0, 0x1
    );
#endif // _COM_UART_

    return(_bytes_write_);
}



//! reading a key used to identify a data
_KEY_ _com_recv_key_(void) {
    _KEY_ _key_ = _kERROR_;

    (void)_com_recv_(
        &_key_, 0x1, sizeof(_key_)
    );

    return(_key_);
}

//! writing a key used to identify a data
void _com_send_key_(_KEY_ _key_) {
    (void)_com_send_(
        &_key_, 0x1, sizeof(_key_)
    );
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
