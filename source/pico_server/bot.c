#include "bot.h"

#include "data.h"

/**
 * @file
 *
 * @brief bot.c
 */

#include "logic.h"

bool _bot_step_(
    _GAME_* _game_, bool _turn_, _POINT_* _old_, _POINT_* _new_
) {
    bool _b_ = 0x0;

    _GAME_ _virtual_ = {};

    if (
        !_game_copy_(&_virtual_, _game_)
    ) {
        return(_b_);
    }

    _DATA_ _checker_ = {};

    if (
        _logic_find_checker_all_(&_checker_, &_virtual_, _turn_)
    ) {
        static int8_t (*_s_[0x2])(char) = {
            &_logic_set_jump_, &_logic_set_move_
        };

        static void* (*_f_[0x2])(void*, _GAME_*, _POINT_, char, _POINT_, int8_t, int8_t) = {
            &_logic_add_jump_, &_logic_add_move_
        };



        size_t _cnt_ = _checker_._cnt_;
        _CHECKER_* _data_ = (_CHECKER_*)_checker_._data_;

        enum _eStep_ { _eJUMP_ = 0x0, _eMOVE_ = 0x1, _eSIZE_ };

        for (
            int8_t j = 0x0; j < _eSIZE_; j += 0x1
        ) {
            _DATA_ _step_ = {};

            for (
                size_t i = 0x0; i < _cnt_; i += 0x1
            ) {
                _CHECKER_ _ch_ = _data_[i];

                int8_t _set_ = _s_[j](_ch_._c_);

                void* _res_ = _logic_all_(
                    &_step_, &_virtual_, _ch_._id_, _ch_._c_, _set_, _f_[j]
                );

                if (!_res_) { break; }
            }

            if (
                _step_._cnt_ != 0x0
            ) {
                _STEP_ _s_ = {};



                size_t _index_ = rand() % _step_._cnt_;

                _s_ = (
                    (_STEP_*)(_step_._data_)
                )[_index_];



                *(_old_) = _s_._old_, *(_new_) = _s_._new_;

                (void)_data_clear_(&_step_);
                _b_ = true; goto linkExit;
            }
        }
    } else {
        return(_b_);
    }

linkExit:
    if (
        !_data_clear_(&_checker_)
    ) {
        /*Code...*/
    }

    if (
        !_game_destroy_(&_virtual_)
    ) {
        /*Code...*/
    }

    return(_b_);
}
