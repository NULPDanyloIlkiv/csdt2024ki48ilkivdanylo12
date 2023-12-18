#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "framework.h"

#include "data.h"

/**
 * @file
 *
 * @brief logic.h
 */

void* _logic_all_(
    void* _data_, _POINT_ _id_, char _c_, int8_t _set_, void* (_f_)(void*, _POINT_, char, _POINT_, int8_t, int8_t)
);

typedef struct {
    size_t _w_, _h_; void** _data_; size_t _cnt_;
} _MASK_;

_MASK_ _logic_create_mask_(
    _MASK_ _data_, size_t _size_
);

_MASK_ _logic_destroy_mask_(
    _MASK_ _data_
);



bool _logic_is_move_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
);

_MASK_ _logic_find_move_all_(
    _MASK_ _data_, _POINT_ _id_, char _c_
);



bool _logic_is_jump_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
);
_MASK_ _logic_find_jump_all_(
    _MASK_ _data_, _POINT_ _id_, char _c_
);



bool _logic_is_jump_all_(
    _POINT_ _id_, char _c_
);

bool _logic_is_jump_at_least_all_(bool _turn_);



bool _logic_create_(void);

bool _logic_destroy_(void);

bool _logic_step_(
    _POINT_ _old_, _POINT_ _new_, _STEP_* _step_
);

#endif // _LOGIC_H_
