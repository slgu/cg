#pragma once
#include "surface.h"
#include "point.h"
class sphere : public surface {
    point o;
    float r;
public:
    sphere(float x, float y, float z, float _r) {
        o = vector (x, y, z);
        norm(o);
        r = _r;
    }
};
