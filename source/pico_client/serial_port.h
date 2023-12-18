#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

#include "framework.h"

#define _Xon_ 0x11
#define _Xoff_ 0x13

#define _UPD_ 0x3

struct _sSerialPort_ {
    const wchar_t*
        wcPortID = NULL;

    HANDLE hPort = NULL;

    DCB dcbSP = { 0x0 };
    COMMTIMEOUTS
        cmtST = { 0x0 };
}; typedef _sSerialPort_ _SERIAL_PORT_;

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

DWORD _com_close_(_SERIAL_PORT_* _port_) {
    DWORD dwError = ERROR_SUCCESS;

    dwError = CloseHandle(
        _port_->hPort
    );

    _port_->hPort = NULL;

    return(dwError);
}

DWORD _com_setup_(_SERIAL_PORT_* _port_,
    DWORD _rate_ = CBR_115200
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

DWORD _com_mask_(_SERIAL_PORT_* _port_, DWORD dwEvtMask = EV_RXCHAR) {
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

bool _com_clear_(_SERIAL_PORT_* _port_) {
    bool _result_ = 0x0;

    _result_ = PurgeComm(_port_->hPort,
        PURGE_RXCLEAR | PURGE_TXCLEAR
    );

    return(_result_);
}

#endif // _SERIAL_PORT_H_
