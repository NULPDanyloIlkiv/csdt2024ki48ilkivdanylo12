#include "bot.h"

#include "game.h"
#include "logic.h"

#include "data.h"

/**
 * @file
 *
 * @brief bot.c
 */

struct _sChecker_ {
    _POINT_ _id_; char _c_;
};

typedef struct _sChecker_ _CHECKER_;

static struct _sBot_ {
    _DATA_ _checker_;
} _bot_ = {};

typedef struct _sBot_ _BOT_;



static size_t _bot_add_checker_(
    int8_t x, int8_t y, char _c_
)
{
    (void)_data_add_(
        &_bot_._checker_, sizeof(_CHECKER_)
    );

    _CHECKER_* _checker_ = (_CHECKER_*)_bot_._checker_._data_;

    _checker_[
        _bot_._checker_._cnt_ - 0x1
    ] = (_CHECKER_){ (_POINT_){ x, y }, _c_ };

    return(_bot_._checker_._cnt_);
}

static size_t _bot_clear_checker_(void) {
    return(
        _data_clear_(&_bot_._checker_)
    );
}

static bool _bot_find_checker_(bool _turn_) {
    bool _b_ = 0x0;

    for (int8_t y = 0x0; y < _get_board_h_(); y += 0x1)
    for (int8_t x = 0x0; x < _get_board_w_(); x += 0x1)
    {
        char _c_ = _get_board_char_(x, y);

        if (
            !(_turn_) ? (_c_ != 'W' && _c_ != 'Q') : (_c_ != 'B' && _c_ != 'K')
        ) { continue; }

        (void)_bot_add_checker_(x, y, _c_);

        _b_ |= true;
    }

    return(_b_);
}



static void* _bot_is_move_(
    void* _data_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    _DATA_* _move_ = (_DATA_*)_data_;

    if (
        _is_inside_board_(_p_.x, _p_.y)
    ) {
        if (
            _logic_is_move_one_(_id_, _c_, _p_)
        ) {
            _data_add_(_move_, sizeof(_STEP_));

            _STEP_* _step_ = (_STEP_*)_move_->_data_;

            _step_[
                _move_->_cnt_ - 0x1
            ] = (_STEP_){ _id_, _p_ };
        }
    }

    return(_data_);
}

static void* _bot_is_jump_(
    void* _data_, _POINT_ _id_, char _c_, _POINT_ _p_, int8_t x, int8_t y
) {
    void* _res_ = _data_;
    if (!_res_) { return(_res_); }

    bool _combo_ = 0x0;

    if (
        _get_flag_() & _fCOMBO_
    ) {
        _STEP_ _mem_ = _get_mem_();

        _combo_ = (_id_.x == _mem_._new_.x) && (_id_.y == _mem_._new_.y);
    }

    _DATA_* _jump_ = (_DATA_*)_data_;

    if (
        _logic_is_jump_one_(_id_, _c_, _p_)
    ) {
        _data_add_(_jump_, sizeof(_STEP_));

        _STEP_* _step_ = (_STEP_*)_jump_->_data_;
        _step_[_jump_->_cnt_ - 0x1] = (_STEP_){
            _id_, (_POINT_){ _p_.x + x, _p_.y + y}
        };

        if (_combo_) { _res_ = NULL; }
    }

    return(_res_);
}



static int8_t (*_s_[0x2])(char) = {
    &_game_set_jump_, &_game_set_move_
};

bool _bot_step_(
    bool _turn_, _POINT_* _old_, _POINT_* _new_
) {
    (void)_bot_clear_checker_();

    if (
        _bot_find_checker_(_turn_)
    ) {
        void* (*_f_[0x2])(void*, _POINT_, char, _POINT_, int8_t, int8_t) = {
            &_bot_is_jump_, &_bot_is_move_
        };



        size_t _cnt_ = _bot_._checker_._cnt_;
        _CHECKER_* _data_ = (_CHECKER_*)_bot_._checker_._data_;

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
                    &_step_, _ch_._id_, _ch_._c_, _set_, _f_[j]
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

                (void)_data_clear_(&_step_); return(true);
            }
        }
    } else {
        return(0x0);
    }

    return(0x0);
}
