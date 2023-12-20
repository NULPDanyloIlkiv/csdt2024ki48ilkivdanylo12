#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "framework.h"

#include "data.h"

/**
 * @file
 *
 * @brief update.h
 */

size_t _update_add_(
    _DATA_* _data_, _CHECKER_ _ch_old_, _CHECKER_ _ch_new_
);

size_t _update_clear_(
	_DATA_* _data_
);

#endif // _UPDATE_H_
