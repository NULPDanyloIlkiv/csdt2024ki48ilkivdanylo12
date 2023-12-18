#include "serial_port.h"

/**
 * @file
 *
 * @brief serial_port.cpp
 *
 * this file stores the implementation of functions for working with the COM port data stream
 */

/**
 * @brief library for working with COM port data stream
 * @author Ilkiv Danylo KI-48
 *
 * @version v1.0.0
 * @date 12/12/24
 *
 * @warning for the application to work, it is necessary to connect the "RP PICO" microcontroller with the server application recorded on it to the COM port!
 */

//! the XON symbol used to receive and transmit data
#define _Xon_ 0x11
//! the XOFF symbol used to receive and transmit data
#define _Xoff_ 0x13

#define _UPD_ 0x3

//! opening the COM port descriptor
DWORD _com_open_(_SERIAL_PORT_* _port_) {
    DWORD dwError = ERROR_SUCCESS;

    _port_->hPort = CreateFile(_port_->wcPortID,
        GENERIC_READ | GENERIC_WRITE, 0x0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
    );

    if (
        _port_->hPort != INVALID_HANDLE_VALUE
    ) {
        fwprintf(stdout,
            L"Port %ls has OPEN.\n", _port_->wcPortID
        );
    } else {
        fwprintf(stderr,
            L"ERROR - failed to OPEN port %ls.\n", _port_->wcPortID
        ); dwError = GetLastError();
    }

    return(dwError);
}

//! closing the COM port descriptor
DWORD _com_close_(_SERIAL_PORT_* _port_) {
    DWORD dwError = ERROR_SUCCESS;

    dwError = CloseHandle(
        _port_->hPort
    );

    _port_->hPort = NULL;

    return(dwError);
}

//! setting the parameters of the COM port
DWORD _com_setup_(_SERIAL_PORT_* _port_,
    DWORD _rate_
)
{
    DWORD dwError = ERROR_SUCCESS;

    ZeroMemory(
        &_port_->dcbSP, sizeof(DCB)
    );
    _port_->dcbSP.DCBlength = sizeof(DCB);



    if (
        !GetCommState(_port_->hPort, &_port_->dcbSP)
    ) {
        fwprintf(stderr,
            L"ERROR - failed to get STATE of port %ls.\n", _port_->wcPortID
        ); dwError = GetLastError(); goto linkExit;
    }

    _port_->dcbSP.BaudRate = _rate_;

    _port_->dcbSP.ByteSize = 0x8;
    _port_->dcbSP.StopBits = ONESTOPBIT;
    _port_->dcbSP.Parity = NOPARITY;

    _port_->dcbSP.fOutX = TRUE;
    _port_->dcbSP.fInX = TRUE;

    _port_->dcbSP.XonChar = _Xon_;
    _port_->dcbSP.XoffChar = _Xoff_;

    _port_->dcbSP.fDtrControl = DTR_CONTROL_ENABLE;

    if (
        !SetCommState(_port_->hPort, &_port_->dcbSP)
    ) {
        fwprintf(stderr,
            L"ERROR - failed to set STATE of port %ls.\n", _port_->wcPortID
        ); dwError = GetLastError(); goto linkExit;
    }



    ZeroMemory(
        &_port_->cmtST, sizeof(COMMTIMEOUTS)
    );

    _port_->cmtST.ReadIntervalTimeout = 128;

    _port_->cmtST.ReadTotalTimeoutConstant = MAXDWORD;
    _port_->cmtST.ReadTotalTimeoutMultiplier = MAXDWORD;

    _port_->cmtST.WriteTotalTimeoutConstant = MAXDWORD;
    _port_->cmtST.WriteTotalTimeoutMultiplier = MAXDWORD;

    if (
        !SetCommTimeouts(_port_->hPort, &_port_->cmtST)
    ) {
        fwprintf(stderr,
            L"ERROR - failed to set TIMEOUT for port %ls.\n", _port_->wcPortID
        ); dwError = GetLastError(); goto linkExit;
    }

linkExit:
    return(dwError);
}

//! setting the COM port mask, which indicates the set of events that should be monitored for the communication device
DWORD _com_mask_(_SERIAL_PORT_* _port_, DWORD dwEvtMask) {
    DWORD dwError = ERROR_SUCCESS;

    if (
        !SetCommMask(_port_->hPort, dwEvtMask)
    )
    {
        fwprintf(stderr,
            L"ERROR - failed to set MASK for port %ls.\n", _port_->wcPortID
        ); dwError = GetLastError();
    }

    return(dwError);
}



//! reading bytes from the COM port data stream
size_t _com_recv_(_SERIAL_PORT_* _port_,
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    uint8_t _update_ = 0x0;

    size_t _bytes_read_ = 0x0, _read_ = 0x0;

    for (;
        _update_ < _UPD_; _update_ += 0x1
    ) {
        _bytes_read_ = 0x0;

        if(
            !ReadFile(_port_->hPort, _buffer_ + _read_, (_size_ * _cnt_) - _read_, (DWORD*)&_bytes_read_, NULL)
        ) {
            fwprintf(stderr,
                L"ERROR - failed to READ data to port %ls. Try again.\n", _port_->wcPortID
            );
        }

        _read_ += _bytes_read_;

        if (
            _read_ == _size_ * _cnt_
        ) { break; }
    }

    return(_bytes_read_);
}

//! writing bytes to the COM port data stream
size_t _com_send_(_SERIAL_PORT_* _port_,
    void* _buffer_, size_t _cnt_, size_t _size_
)
{
    uint8_t _update_ = 0x0;

    size_t _bytes_write_ = 0x0, _write_ = 0x0;

    for (;
        _update_ < _UPD_; _update_ += 0x1
    ) {
        if(
            !WriteFile(_port_->hPort, _buffer_ + _write_, (_size_ * _cnt_) - _write_, (DWORD*)&_bytes_write_, NULL)
        ) {
            fwprintf(stderr,
                L"ERROR - failed to WRITE data to port %ls. Try again.\n", _port_->wcPortID
            );
        }

        _write_ += _bytes_write_;

        if (
            _write_ == _size_ * _cnt_
        ) { break; }
    }

    return(_bytes_write_);
}

//! clearing bytes from the COM port data stream
bool _com_clear_(_SERIAL_PORT_* _port_) {
    bool _result_ = 0x0;

    _result_ = PurgeComm(_port_->hPort,
        PURGE_RXCLEAR | PURGE_TXCLEAR
    );

    return(_result_);
}



//! reading the key used to identify the data
_KEY_ _com_recv_key_(
    _SERIAL_PORT_* _port_
) {
    _KEY_ _key_ = _kERROR_;

    size_t _size_ = _com_recv_(
        _port_, &_key_, 0x1, sizeof(_key_)
    );

    if (!_size_)
        fwprintf(stderr,
            L"__ERROR__ - Failed to READ a KEY.\n"
        );

    return(_key_);
}

//! writing the key used to identify the data
void _com_send_key_(
    _SERIAL_PORT_* _port_, _KEY_ _key_
) {
    (void)_com_send_(
        _port_, &_key_, 0x1, sizeof(_key_)
    );
}



 //! writing a message in the COM port data stream, consisting of a key and data
size_t _com_send_data_(_SERIAL_PORT_* _port_,
    _KEY_ _key_, void* _buffer_, size_t _cnt_, size_t _size_
)
{
    size_t _bytes_write_ = 0x0;

    _bytes_write_ += _com_send_(
        _port_, &_key_, 0x1, sizeof(_key_)
    );
       _bytes_write_ += _com_send_(
        _port_, _buffer_, _cnt_, _size_
    );

    return(_bytes_write_);
}

//! reading a string-message from the COM port data stream, consisting of a key and data
size_t _com_message_(
    _SERIAL_PORT_* _port_
)
{
    char _message_[_MSG_SIZE_] = { 0x0 };

    size_t _bytes_recv_ = _com_recv_(
        _port_, _message_, 0x1, sizeof(_message_)
    );

    fwprintf(stderr,
        L"__BYTES_RECV__ : %i | _MESSAGE_ : %s\n", _bytes_recv_, _message_
    );

    return(_bytes_recv_);
}
