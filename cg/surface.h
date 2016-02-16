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
