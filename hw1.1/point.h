#pragma once
#include "vect.h"
class point {
    float x;
    float y;
    float z;
public:
    point(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    point() {
        x = 0;
        y = 0;
        z = 0;
    }
    vect operator-(const point & o) {
        return vect(x - o.x, y - o.y, z - o.z);
    }
    vect operator+(const point & o) {
        return vect(x + o.x, y + o.y, z + o.z);
    }
};
