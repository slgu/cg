//
//  light.hpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include "point.h"
#include "material.hpp"
#include "helper.h"
#include "ray.h"
//point light
class light {
public:
    virtual void calculate_rgb(point & intersect_p, vect & n, material & m,
                               vect & inter, float & r, float & g, float & b) = 0;
    virtual bool check_ambient() = 0;
    virtual bool check_area() = 0;
    virtual ray generate_shadow_ray(const point & inter_p, float & t) = 0;
};

class plight : public light {
public:
    point p;
    float color[3];
    //float intensity;
    plight(float x, float y, float z, float r, float g, float b): p(x, y, z) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    
    virtual void calculate_rgb(point & intersect_p, vect & n, material & m,
                               vect & inter, float & r, float & g, float & b);
    virtual bool check_ambient();
    virtual bool check_area() {
        return false;
    }
    
    virtual ray generate_shadow_ray(const point & inter_p, float & t);
};

class alight: public light {
public:
    float color[3];
    alight(float r, float g, float b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    virtual void calculate_rgb(point & intersect_p, vect & n, material & m,
                               vect & inter, float & r, float & g, float & b);
    virtual bool check_ambient();
    virtual bool check_area() {
        return false;
    }
    virtual ray generate_shadow_ray(const point & inter_p, float & t);
};

//area light
class slight: public light {
public:
    point p;
    point randp; // now sample position of area light
    float color[3];
    vect u;
    vect v;/* u v is the directions of the square */
    vect n;
    float len; /* length of the area light */
    slight(float x, float y, float z, float nx, float ny, float nz, float ux,
           float uy, float uz, float _len, float r, float g, float b):p(x,y,z) {
        u = vect(ux, uy, uz);
        n = vect(nx, ny, nz); //normal vector
        v = cross_product(u, n);
        norm(v);
        norm(u);
        len= _len;
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
    
    virtual bool check_area() {
        return true;
    }
    virtual void calculate_rgb(point & intersect_p, vect & n, material & m,
                               vect & inter, float & r, float & g, float & b);
    virtual bool check_ambient();
    virtual ray generate_shadow_ray(const point & inter_p, float & t);
    ray generate_area_shadow_ray(const point & inter_p, int total, int o, int k, float & t);
};

#endif /* light_hpp */
