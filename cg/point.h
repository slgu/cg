#pragma once
#include "vect.h"
class point {
public:
    float x;
    float y;
    float z;
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
    
    void debug() {
        std::cout << "point " << x << "," << y << "," << z << std::endl;
    }
    vect operator-(const point & o) const{
        return vect(x - o.x, y - o.y, z - o.z);
    }
    vect operator+(const point & o) const{
        return vect(x + o.x, y + o.y, z + o.z);
    }
    point operator+(const vect & o) const{
        return point(x + o.x, y + o.y, z + o.z);
    }
};
