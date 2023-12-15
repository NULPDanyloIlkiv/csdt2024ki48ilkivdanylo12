#pragma once
#include "vec_mat.h"

/**
 * @file
 *
 * @brief vec_mat_functions.h
 */

float clamp(
    float value, float min, float max
);

float sign(float a);

float step(
    float edge, float x
);

float length(vec2f const& v);
