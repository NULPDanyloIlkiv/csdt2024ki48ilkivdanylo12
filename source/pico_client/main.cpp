#include <iostream>
#include <cassert>

#include "serial_port.h"
#define _PORT_ID_ (wchar_t*)L"COM4"

#include "timer.h"

#define _MSG_SIZE_ 128

void _open_(
    _SERIAL_PORT_* _port_
)
{
    _com_open_(_port_);

    assert(
        _port_->hPort != INVALID_HANDLE_VALUE
    );

    _com_setup_(
        _port_, CBR_115200
    );

    _com_mask_(
        _port_, EV_RXCHAR
    );
}
void _close_(
    _SERIAL_PORT_* _port_
) {
    _com_close_(_port_);
}

void _ping_(
    _SERIAL_PORT_* _port_
) {
    char _char_[
        _MSG_SIZE_
    ] = "__HELLO_FROM_CLIENT__";

    size_t _bytes_send_ = _com_send_(
        _port_, _char_, 0x1, sizeof(_char_)
    );

    fwprintf(stderr,
        L"__BYTES_SEND__ : %i | _CHAR_ : %s\n", _bytes_send_, _char_
    );



    memset(
        _char_, '\0', sizeof(_char_)
    );



    size_t _bytes_recv_ = _com_recv_(
        _port_, _char_, 0x1, sizeof(_char_)
    );

    fwprintf(stderr,
        L"__BYTES_RECV__ : %i | _CHAR_ : %s\n", _bytes_recv_, _char_
    );
}

int main(void) {
    _SERIAL_PORT_ m_sp = { _PORT_ID_ };

    _open_(&m_sp);

    _cTimer_ _timer_;

    while(0x1) {
        _timer_._begin_();

            Sleep(128); _ping_(&m_sp); Sleep(128);

        _timer_._end_();

        fwprintf(
            stderr, L"__ELAPSED_TIME__ : %Lf\n", _timer_._elapsed_time_()
        );
    }

    _close_(&m_sp);

    return(0x0);
}
