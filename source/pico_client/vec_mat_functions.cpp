#include "vec_mat_functions.h"

#include <cmath>

/**
 * @file
 *
 * @brief vec_mat_functions.cpp
 */

float clamp(
    float value, float min, float max
) { return fmax(fmin(value, max), min); }

float sign(float a)
{ return (float)((0.f < a) - (a < 0.f)); }

float step(
    float edge, float x
) { return (float)(x > edge); }



float length(vec2f const& v)
{ return (float)sqrtf((float)v.x * (float)v.x + (float)v.y * (float)v.y); }
