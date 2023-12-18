#include <stdlib.h>
#include "pico/stdlib.h"

#include "pico/bootrom.h"

const int _gLED_PIN_ = PICO_DEFAULT_LED_PIN;

#include "serial_port.h"
#define _MSG_SIZE_ 128

void _ping_(void) {
    char _char_[
        _MSG_SIZE_
    ];

    (void)_com_recv_(
        _char_, 0x1, sizeof(_char_)
    );

    memset(
        _char_, '\0', sizeof(_char_)
    );

    strcpy(
        _char_, "__HELLO_FROM_SERVER__"
    );

    (void)_com_send_(
        _char_, 0x1, sizeof(_char_)
    );
}

int main() {
    stdio_init_all();

    gpio_init(_gLED_PIN_);
    gpio_set_dir(
        _gLED_PIN_, GPIO_OUT
    );

    gpio_put(
        _gLED_PIN_, 0x1
    );

    while(0x1) {
        _ping_();
    }

    gpio_put(
        _gLED_PIN_, 0x0
    );

    reset_usb_boot(
        0x0, 0b0001
    );

    return(0x0);
}
