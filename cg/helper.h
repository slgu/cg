#pragma once
#include "vect.h"
#include <cmath>
inline void norm(vect &d) {
    float sum = d.x * d.x + d.y * d.y + d.z * d.z;
    sum = sqrt(sum);
    d.x = d.x / sum;
    d.y = d.y / sum;
    d.z = d.z / sum;
}

inline vect cross_product(const vect & v1, const vect & v2) {
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vect(x, y, z);
}

inline float inner_product(const vect & v1, const vect & v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
