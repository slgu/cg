#pragma once
#include "point.h"
#include "vect.h"
class ray {
public:
    point origin;
    vect dir;
    ray(){

    }
    ray(point _origin, vect _dir) {
        origin = _origin;
        dir = _dir;
    }
    point get_t(float t) {
        return origin + dir * t;
    }
};
