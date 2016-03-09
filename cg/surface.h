#pragma once
#include "helper.h"
#include "ray.h"
#include "material.hpp"
#include <memory>
#include <limits>
#include <cassert>
/*definition for aabb*/
class AABB;


class surface {
public:
    std::shared_ptr <material> m;
    virtual bool intersect(ray & r, float & t) = 0;
    virtual void set_material(std::shared_ptr <material> _m) {
        m = _m;
    }
    virtual vect get_n(const point & p)  = 0;
    virtual std::string debug() = 0;
    virtual std::shared_ptr <AABB> get_aabb() = 0;
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
    std::shared_ptr <AABB> get_aabb();
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
    
    std::shared_ptr <AABB> get_aabb();
};

/*definition for triangle*/
class tri : public surface {
private:
    point p[3];
    vect nrml;//norm of the plane of triangle
public:
    tri(float x1, float y1, float z1,
        float x2, float y2, float z2, float x3, float y3, float z3) {
        p[0].x = x1;
        p[0].y = y1;
        p[0].z = z1;
        p[1].x = x2;
        p[1].y = y2;
        p[1].z = z2;
        p[2].x = x3;
        p[2].y = y3;
        p[2].z = z3;
        //calculate nrml using cross product
        vect v2_1 = p[1] - p[0];
        vect v3_2 = p[2] - p[1];
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
    
    std::shared_ptr <AABB> get_aabb();
};

class AABB :public surface{
public:
    float mmin[3];
    float mmax[3];
    /* a surface it contains */
    std::shared_ptr <surface> obj;
    AABB() {
        //init is a box without anything
        for (int i = 0; i < 3; ++i)
            mmin[i] = std::numeric_limits<float>::max();
        for (int i = 0; i < 3; ++i)
            mmax[i] = std::numeric_limits<float>::min();
    }
    AABB(float _xmin, float _xmax, float _ymin, float _ymax, float _zmin, float _zmax){
        mmin[0] = _xmin;
        mmin[1] = _ymin;
        mmin[2] = _zmin;
        mmax[0] = _xmax;
        mmax[1] = _ymax;
        mmax[2] = _zmax;
        obj = nullptr;
    }
    virtual vect get_n(const point & p) {
        if (check_eps(p.x - mmin[0]))
            return vect(-1,0,0);
        if (check_eps(p.x - mmax[0]))
            return vect(1,0,0);
        if (check_eps(p.y - mmin[1]))
            return vect(0,-1,0);
        if (check_eps(p.y - mmax[1]))
            return vect(0,1,0);
        if (check_eps(p.z - mmin[2]))
            return vect(0,0,-1);
        if (check_eps(p.z - mmax[2]))
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
        m = obj->m; // set material also
    }
    float get_volume() {
        float res = 1;
        for (int i = 0; i < 3; ++i)
            res *= (mmax[i] - mmin[i]);
        return res;
    }
    std::shared_ptr <AABB> get_aabb() {
        assert(false);
        std::shared_ptr <AABB> res(this);
        return res;
    }
};

