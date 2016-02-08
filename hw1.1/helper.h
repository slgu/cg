#pragma once
#include "vector.h"
#include <cmath>
inline void norm(vector &d) {
    float sum = d.x * d.x + d.y * d.y + d.z * d.z;
    sum = sqrt(sum);
    d.x = d.x / sum;
    d.y = d.y / sum;
    d.z = d.z / sum;
}

inline vector cross_product(vector & v1, vector & v2) {
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vector(x, y, z);
}
