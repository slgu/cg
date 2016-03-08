#pragma once
#include "helper.h"
#include "ray.h"
#include "material.hpp"
#include <memory>
#include <cassert>
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

/*definition for aabb*/
class AABB : public surface{
private:
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    /* a surface it contains */
    std::shared_ptr <surface> obj;
public:
    AABB(float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax):xmin(_xmin), xmax(_xmax)
    , ymin(_ymin), ymax(_ymax), zmin(_zmin), zmax(_zmax){
        obj = nullptr;
    }
    virtual vect get_n(const point & p) {
        if (check_eps(p.x - xmin))
            return vect(-1,0,0);
        if (check_eps(p.x - xmax))
            return vect(1,0,0);
        if (check_eps(p.y - ymin))
            return vect(0,-1,0);
        if (check_eps(p.y - ymax))
            return vect(0,1,0);
        if (check_eps(p.z - zmin))
            return vect(0,0,-1);
        if (check_eps(p.z - zmax))
            return vect(0,0,1);
        //maybe wrong into this position
        assert(false);
        return vect(0,0,0);
    }
    std::string debug() {
        return "AABB";
    }
    virtual bool intersect(ray & _r, float & t);
    void set_obj(std::shared_ptr <surface> _obj) {
        obj = _obj;
    }
};