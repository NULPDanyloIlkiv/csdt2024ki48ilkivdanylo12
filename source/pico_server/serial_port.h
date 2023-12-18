#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include <stdio.h>
#include "pico/stdio.h"

/*
 *  "serial_port.h"
 *
 *  Library for working with COM port data stream.
 *
 *  It's necessary to specify a global visibility
 *  area for some library functions <pico/stdio.h>.
 *
 *  bool stdio_put_string(
 *     const char *s, int len, bool newline, bool no_cr
 *  );
 *
 *  int stdio_get_until(
 *     char *buf, int len, absolute_time_t until
 *  );
 *
*/

bool stdio_put_string(
    const char*, int, bool, bool
);

int stdio_get_until(
    char*, int, absolute_time_t
);



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

#endif // _SERIAL_PORT_H_
