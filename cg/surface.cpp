//
//  surface.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "surface.h"
#include <limits>
bool sphere::intersect(ray & _r, float & t) {
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

bool plane::intersect(ray & _r, float & t) {
    float d_n  = inner_product(n, _r.dir);
    if (d_n == 0) { //maybe eps
        return false;
    }
    float p_n = inner_product(_r.origin, n);
    t = -(p_n + d) / d_n;
    if (t < 0)
        return false;
    return true;
}

bool tri::intersect(ray & _r, float & t) {
    //calculate t first
    t = inner_product(p1 - _r.origin, nrml) / inner_product(_r.dir, nrml);
    if (t < 0)
        return false;
    //check inside the tri
    point x = _r.get_t(t);
    vect v2_1 = p2 - p1;
    vect v3_2 = p3 - p2;
    vect v1_3 = p1 - p3;
    vect vx_1 = x - p1;
    vect vx_2 = x - p2;
    vect vx_3 = x - p3;
    /*special judge for debug
    if(abs(x.x) > 500 || abs(x.z) > 500)
        return false;
    return true;
     */
    if (inner_product(cross_product(v2_1, vx_1), nrml) > 0
        && inner_product(cross_product(v3_2, vx_2), nrml) > 0
        && inner_product(cross_product(v1_3, vx_3), nrml) > 0) {
        return true;
    }
    return false;
}

bool AABB::intersect(ray &_r, float &t){
    float t_l = std::numeric_limits<float>::min();
    float t_r = std::numeric_limits<float>::max();
    float tmin;
    float tmax;
    /* stupid QAQ I need use array at first */
    if (_r.dir.x == 0) {
        if (_r.origin.x > xmax || _r.origin.x < xmin)
            return false;
    }
    else {
        tmin = (xmin - _r.origin.x) / _r.dir.x;
        tmax = (xmax - _r.origin.x) / _r.dir.x;
        if (tmin > tmax)
            swap(tmin, tmax);
        if (tmin > t_l)
            t_l = tmin;
        if (tmax < t_r)
            t_r = tmax;
    }
    if (t_l > t_r)
        return false;
    
    if (_r.dir.y == 0) {
        if (_r.origin.y > ymax || _r.origin.y < ymin)
            return false;
    }
    else {
        tmin = (ymin - _r.origin.y) / _r.dir.y;
        tmax = (ymax - _r.origin.y) / _r.dir.y;
        if (tmin > tmax)
            swap(tmin, tmax);
        if (tmin > t_l)
            t_l = tmin;
        if (tmax < t_r)
            t_r = tmax;
    }
    if (t_l > t_r)
        return false;
    
    if (_r.dir.z == 0) {
        if (_r.origin.z > zmax || _r.origin.z < zmin)
                return false;
        }
        else {
            tmin = (zmin - _r.origin.z) / _r.dir.z;
            tmax = (zmax - _r.origin.z) / _r.dir.z;
            if (tmin > tmax)
                swap(tmin, tmax);
            if (tmin > t_l)
                t_l = tmin;
            if (tmax < t_r)
                t_r = tmax;
        }
    if (t_l > t_r)
        return false;
    return true;
}
