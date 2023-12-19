#include <stdlib.h>
#include "pico/stdlib.h"

#include "pico/bootrom.h"

const int _gLED_PIN_ = PICO_DEFAULT_LED_PIN;

#include "server.h"

#include "game.h"

#include "data.h"

/**
 * @file
 *
 * @brief main.c
 *
 * this file stores the entry point of the server application
 */

 /**
 * @brief server application for the "checkers" game on pico
 * @author Ilkiv Danylo KI-48
 *
 * @version v1.0.0
 * @date 12/12/24
 */

//! entry point to the application
int main() {
    stdio_init_all();

    gpio_init(_gLED_PIN_);
    gpio_set_dir(
        _gLED_PIN_, GPIO_OUT
    );

    gpio_put(
        _gLED_PIN_, 0x1
    );



    bool _state_ = true;

    //! server loop
    while(_state_) {
        _state_ = true;

        _GAME_ _game_ = {};

        _state_ &= _game_create_(
            &_game_, _gSIZE_W_, _gSIZE_H_, _gDATA_
        );

        _state_ &= _server_loop_(&_game_);

        _state_ &= _game_destroy_(&_game_);
    }



    gpio_put(
        _gLED_PIN_, 0x0
    );

    //! reboot...
    reset_usb_boot(
        0x0, 0b0001
    );

    return(0x0);
}
