#pragma once
#include "helper.h"
#include "ray.h"
#include "material.hpp"
class surface {
public:
    std::shared_ptr <material> m;
    virtual bool intersect(ray & r, float & t) = 0;
    virtual void set_material(std::shared_ptr <material> _m) {
        m = _m;
    }
    virtual vect get_n(const point & p)  = 0;
};

class sphere : public surface {
    point o;
    float r;
public:
    sphere(float x, float y, float z, float _r) {
        o = point(x, y, z);
        r = _r;
    }
    virtual bool intersect(ray & _r, float & t);
    virtual vect get_n(const point & p) {
        return p - o;
    }
};

class plane : public surface {
    vect n;
    float d;
public:
    plane(float nx, float ny, float nz, float _d)
    :n(nx, ny, nz), d(_d) {
        norm(n);
    }
    virtual bool intersect(ray & _r, float & t);
    virtual vect get_n(const point & p) {
        return n;
    }
};