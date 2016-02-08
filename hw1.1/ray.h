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
};
