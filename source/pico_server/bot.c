#include "bot.h"

#include "data.h"

/**
 * @file
 *
 * @brief bot.c
 */

#include "logic.h"

static const int8_t _alive_value_ = 0x3;

static const int32_t _W_or_B_value_ = 64, _Q_or_K_value_ = 128;

static const int32_t _Y_value_[] = {
    0x2, 0x1, 0x3, 0x3, 0x3, 0x1, 0x2, 0x3
};

//! a function that calculates a price of a checker
static int32_t _pay_(
    int x, int y, char _c_, bool _turn_
) {
    int32_t _score_ = 0x0;

    int8_t _sign_ = _turn_ ? -0x1 : +0x1;

    switch(_c_)
    {

    case('W'): {
        _score_ += _sign_ * (
            _Y_value_[y] * _W_or_B_value_
        ) * _turn_ ? 0x1 : _alive_value_;
    } break;
    case('B'): {
        _score_ -= _sign_ * (
            _Y_value_[(_gSIZE_H_ - 0x1) - y] * _W_or_B_value_
        ) * _turn_ ? _alive_value_ : 0x1;
    } break;

    case('Q'): {
        _score_ += _sign_ * (
            _Q_or_K_value_
        ) * _turn_ ? 0x1 : _alive_value_;
    } break;
    case('K'): {
        _score_ -= _sign_ * (
            _Q_or_K_value_
        ) * _turn_ ? _alive_value_ : 0x1;
    } break;

    default:
        break;
    }

    return(_score_);
}

//! a function that evaluates a price of a move
static int32_t _price_(
    _BOARD_ _board_, bool _turn_
) {
    int32_t _score_ = 0x0;

    for (int8_t y = 0x0; y < _board_._h_; y += 0x1)
    for (int8_t x = 0x0; x < _board_._w_; x += 0x1)
    {
        char _c_ = _get_board_char_(
            _board_, x, y
        );

        _score_ += _pay_(
            x, y, _c_, _turn_
        );
    }

    return(_score_);
}



/**
 * @brief a function that implements the algorithm for choosing one of the possible moves.
 *
 * the "minimax" algorithm chooses the best possible move
 */
static int32_t _minimax_(
    _GAME_ _game_, bool _turn_, _POINT_* _p_old_, _POINT_* _p_new_, int8_t _depth_, bool _min_max_
) {
    const int32_t _infinity_ = _min_max_ ? INT_MIN : INT_MAX;

    //! if a recursion depth is 0 - find a price of a move
    if (!_depth_) {
        int32_t _score_ = _price_(
            _game_._board_, _turn_
        );

        return(_score_);
    }

    static int8_t (*_s_[0x2])(char) = {
        &_logic_set_jump_, &_logic_set_move_
    };

    static void* (*_f_[0x2])(void*, _GAME_*, _POINT_, char, _POINT_, int8_t, int8_t) = {
        &_logic_add_jump_, &_logic_add_move_
    };

    enum _eStep_ { _eJUMP_ = 0x0, _eMOVE_ = 0x1, _eSIZE_ };

    int32_t _min_max_score_ = _infinity_;

    for (
        int8_t j = 0x0; j < _eSIZE_; j += 0x1
    ) {
        _DATA_ _checker_ = {}, _step_ = {};

        //! looking for all checkers
        if (
            !_logic_find_checker_all_(&_checker_, &_game_, _game_._flag_i_ & _fTURN_)
        ) { return(_infinity_); }

        for (
            size_t i = 0x0; i < _checker_._cnt_; i += 0x1
        ) {
            _CHECKER_* _data_ = (_CHECKER_*)_checker_._data_;

            _CHECKER_ _ch_ = _data_[i];

            int8_t _set_ = _s_[j](_ch_._c_);

            //! looking for all possible moves for checkers on the board
            void* _res_ = _logic_all_(
                &_step_, &_game_, _ch_._id_, _ch_._c_, _set_, _f_[j]
            );

            if (!_res_) { break; }
        }

        (void)_data_clear_(&_checker_);



        for (
            size_t i = 0x0; i < _step_._cnt_; i += 0x1
        ) {
            //! model of a map and objects in memory
            _GAME_ _virtual_ = _game_;

            _STEP_ _s_in_ = {}, _s_out_ = {};

            _s_in_ = (
                (_STEP_*)(_step_._data_)
            )[i];



            _DATA_ _update_ = {};

            bool _old_turn_ = _virtual_._flag_i_ & _fTURN_;

            //! make a step on a board
            _logic_step_(
                &_virtual_, _s_in_._old_, _s_in_._new_, &_s_out_, &_update_
            );

            bool _new_turn_ = _virtual_._flag_i_ & _fTURN_;



            //! if a game is over - choose a step
            if (
                _virtual_._flag_i_ & _fGAME_OVER_
            ) {
                *(_p_old_) = _s_out_._old_, *(_p_new_) = _s_out_._new_;

                const int32_t
                    _p_infinity_ = _min_max_ ? INT_MIN : INT_MAX;
                const int32_t
                    _m_infinity_ = _min_max_ ? INT_MAX : INT_MIN;

                _min_max_score_ = (_turn_ != (_virtual_._flag_i_ & _fTURN_) ? _m_infinity_ + _depth_ : _p_infinity_ - _depth_);

                (void)_data_clear_(&_update_); goto linkGameOver;
            }



            //! call the "minimax" function recursively and get a price of a step
            int32_t _score_ = _minimax_(
                _virtual_, _turn_, &_s_in_._old_, &_s_in_._new_, _depth_ - 0x1, _old_turn_ != _new_turn_ ? (_min_max_ ^ 0x1) : _min_max_
            );

            //! if this move is better than a best move, then this move is a best move
            if (
                _min_max_ ? _score_ >= _min_max_score_ : _score_ <= _min_max_score_
            ) { *(_p_old_) = _s_out_._old_, *(_p_new_) = _s_out_._new_; }

            _min_max_score_ = _min_max_ ? MAX(_score_, _min_max_score_) : MIN(_score_, _min_max_score_);



            //! cancel all steps that have made
            for(
                size_t _cnt_ = _update_._cnt_; _cnt_ != 0x0; _cnt_ -= 0x1
            ) {
                _UPDATE_* _data_ = (_UPDATE_*)_update_._data_;

                _game_update_cancel_(
                    &_game_, _data_[_cnt_ - 0x1]
                );
            }

            (void)_data_clear_(&_update_);
        }

    linkGameOver:
        if (_step_._cnt_) {
            (void)_data_clear_(&_step_); goto linkExit;
        }
    }

linkExit:
    return(_min_max_score_);
}

/**
 * @brief a function that implements the algorithm for choosing one of the possible moves.
 *
 * the "random" algorithm chooses one of the possible moves at random
 */
static void _random_(
    _GAME_ _game_, bool _turn_, _POINT_* _p_old_, _POINT_* _p_new_
) {
    _DATA_ _checker_ = {};

    //! looking for all checkers
    if (
        _logic_find_checker_all_(&_checker_, &_game_, _turn_)
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

                //! looking for all possible moves for checkers on the board
                void* _res_ = _logic_all_(
                    &_step_, &_game_, _ch_._id_, _ch_._c_, _set_, _f_[j]
                );

                if (!_res_) { break; }
            }

            if (
                _step_._cnt_ != 0x0
            ) {
                _STEP_ _s_ = {};


                //! choosing a random move from all possible moves
                size_t _index_ = rand() % _step_._cnt_;

                _s_ = (
                    (_STEP_*)(_step_._data_)
                )[_index_];



                *(_p_old_) = _s_._old_, *(_p_new_) = _s_._new_;

                (void)_data_clear_(&_step_); goto linkExit;
            }
        }
    }

linkExit:
    if (
        !_data_clear_(&_checker_)
    ) {
        /*Code...*/
    }
}

/**
 * @brief a function that implements the choosing one of the possible moves
 *
 * as a result, the function returns the coordinates of cells A and B.
 * a cell A is a cell on which a checker standing. a cell B is a cell on which you need to move a checker.
 */
bool _bot_step_(
    _GAME_* _game_, bool _turn_, _POINT_* _p_old_, _POINT_* _p_new_
) {
    _GAME_ _virtual_ = {};

    if (
        !_game_copy_(&_virtual_, _game_)
    ) {
        return(0x0);
    }

    _minimax_(_virtual_, _turn_, _p_old_, _p_new_, 0x3, !_turn_);

    if (
        !_game_destroy_(&_virtual_)
    ) {
        return(0x0);
    }

    return(true);
}
