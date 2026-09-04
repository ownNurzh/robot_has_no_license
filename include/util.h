#pragma once

#include <cmath>

#include "raylib.h"

inline float to_radians(float deg) {
    return deg * DEG2RAD;
}

inline float to_deg(float rad) {
    return rad * RAD2DEG;
}

inline Vector2 get_direction_from_angle(float angle) {
    float radians = to_radians(angle);
    return {cosf(radians) ,-sinf(radians)};
}

inline float cross(Vector2 a,Vector2 b) {
    return a.x * b.y - a.y * b.x;
}