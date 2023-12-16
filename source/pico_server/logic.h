#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "framework.h"

#include "data.h"

/**
 * @file
 *
 * @brief logic.h
 */

typedef struct {
    size_t _w_, _h_; bool** _data_; size_t _cnt_;
} _MASK_MOVE_;

_MASK_MOVE_ _logic_create_mask_move_(
    _MASK_MOVE_ _data_
);

_MASK_MOVE_ _logic_clear_mask_move_(
    _MASK_MOVE_ _data_
);

_MASK_MOVE_ _logic_destroy_mask_move_(
    _MASK_MOVE_ _data_
);



bool _logic_is_move_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
);

_MASK_MOVE_ _logic_find_move_all_(
    _MASK_MOVE_ _data_, _POINT_ _id_, char _c_
);



typedef struct {
    size_t _w_, _h_; _POINT_*** _data_; size_t _cnt_;
} _MASK_JUMP_;

_MASK_JUMP_ _logic_create_mask_jump_(
    _MASK_JUMP_ _data_
);

_MASK_JUMP_ _logic_clear_mask_jump_(
    _MASK_JUMP_ _data_
);

_MASK_JUMP_ _logic_destroy_mask_jump_(
    _MASK_JUMP_ _data_
);



bool _logic_is_jump_one_(
    _POINT_ _id_, char _c_, _POINT_ _p_
);
_MASK_JUMP_ _logic_find_jump_all_(
    _MASK_JUMP_ _data_, _POINT_ _id_, char _c_
);



bool _logic_is_jump_at_least_all_(bool _s_);



bool _logic_create_(void);

bool _logic_destroy_(void);

bool _logic_step_(
    _POINT_ _old_, _POINT_ _new_, _STEP_* _step_
);

#endif // _LOGIC_H_
