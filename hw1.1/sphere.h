#pragma once
#include "surface.h"
#include "point.h"
class sphere : public surface {
    point o;
    float r;
public:
    sphere(float x, float y, float z, float _r) {
        o = point(x, y, z);
        r = _r;
    }
    virtual bool intersect(ray & _r) {
        vect sub = _r.origin - o;
        float a = inner_product(_r.dir, sub);
        float b = inner_product(_r.dir, _r.dir);
        float c = inner_product(sub, sub);
        return (a * a - b * (c - r * r)) >= 0;
    }
};
