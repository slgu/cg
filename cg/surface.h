#pragma once
#include "helper.h"
#include "ray.h"
#include "material.hpp"
#include <memory>
class surface {
public:
    std::shared_ptr <material> m;
    virtual bool intersect(ray & r, float & t) = 0;
    virtual void set_material(std::shared_ptr <material> _m) {
        m = _m;
    }
    virtual vect get_n(const point & p)  = 0;
    virtual std::string debug() = 0;
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
    std::string debug() {
        return "sphere";
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
    std::string debug() {
        return "plane";
    }
};
/*definition for triangle*/
class tri : public surface {
private:
    point p1;
    point p2;
    point p3;
    vect nrml;//norm of the plane of triangle
public:
    tri(float x1, float y1, float z1,
        float x2, float y2, float z2,
        float x3, float y3, float z3)
    :p1(x1, y1, z1), p2(x2, y2, z2), p3(x3, y3, z3){
        //calculate nrml using cross product
        vect v2_1 = p2 - p1;
        vect v3_2 = p3 - p2;
        nrml = cross_product(v2_1, v3_2);
        norm(nrml);
    }
    virtual bool intersect(ray & _r, float & t);
    virtual vect get_n(const point & p) {
        return nrml;
    }
    std::string debug() {
        return "tri";
    }
};