#pragma once
#include "surface.h"
#include "point.h"
#include <cmath>
class sphere : public surface {
    point o;
    float r;
public:
    sphere(float x, float y, float z, float _r) {
        o = point(x, y, z);
        r = _r;
    }
    virtual bool intersect(ray & _r, float & t) {
        vect sub = _r.origin - o;
        float a = inner_product(_r.dir, sub);
        float b = inner_product(_r.dir, _r.dir);
        float c = inner_product(sub, sub);
        float check = (a * a - b * (c - r * r));
        if (check < 0) return false;
        float t1 = (-a + sqrt(check)) / b;
        float t2 = (-a - sqrt(check)) / b;
        if (t1 < 0 && t2 < 0) return false;
        if (t1 < t2) {
            t = t1;
        }
        else {
            t = t2;
        }
        return true;
    }
    virtual vect get_n(const point & p) {
        return p - o;
    }
};
