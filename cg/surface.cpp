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
    t = inner_product(p[0] - _r.origin, nrml) / inner_product(_r.dir, nrml);
    if (t < 0)
        return false;
    //check inside the tri
    point x = _r.get_t(t);
    vect v2_1 = p[1] - p[0];
    vect v3_2 = p[2] - p[1];
    vect v1_3 = p[0] - p[2];
    vect vx_1 = x - p[0];
    vect vx_2 = x - p[1];
    vect vx_3 = x - p[2];
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
        if (_r.origin.x > mmax[0] || _r.origin.x < mmin[0])
            return false;
    }
    else {
        tmin = (mmin[0] - _r.origin.x) / _r.dir.x;
        tmax = (mmax[0] - _r.origin.x) / _r.dir.x;
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
        if (_r.origin.y > mmax[1] || _r.origin.y < mmin[1])
            return false;
    }
    else {
        tmin = (mmin[1] - _r.origin.y) / _r.dir.y;
        tmax = (mmax[1] - _r.origin.y) / _r.dir.y;
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
        if (_r.origin.z > mmax[2] || _r.origin.z < mmin[2])
                return false;
    }
    else {
        tmin = (mmin[2] - _r.origin.z) / _r.dir.z;
        tmax = (mmax[2] - _r.origin.z) / _r.dir.z;
        if (tmin > tmax)
            swap(tmin, tmax);
        if (tmin > t_l)
            t_l = tmin;
        if (tmax < t_r)
            t_r = tmax;
    }
    if (t_l > t_r)
        return false;
    if (t_r < 0)
        return false;
    t = t_l;
    return true;
}

std::shared_ptr <AABB> sphere::get_aabb() {
    std::shared_ptr <AABB> res(new AABB(o.x - r, o.x + r, o.y - r, o.y + r, o.z - r, o.z + r));
    return res;
}

std::shared_ptr <AABB> plane::get_aabb() {
    float float_min = std::numeric_limits<float>::min();
    float float_max = std::numeric_limits<float>::max();
    std::shared_ptr <AABB> res(new AABB(float_min, float_max, float_min, float_max, float_min, float_max));
    return res;
}

std::shared_ptr <AABB> tri::get_aabb() {
    float float_min = std::numeric_limits<float>::min();
    float float_max = std::numeric_limits<float>::max();
    float mmin[3], mmax[3];
    for (int i = 0; i < 3; ++i) {
        mmin[i] = float_max;
        mmax[i] = float_min;
    }
    for (int i = 0; i < 3; ++i) {
        //x
        if(p[i].x < mmin[0])
            mmin[0] = p[i].x;
        if(p[i].x > mmax[0])
            mmax[0] = p[i].x;
        //y
        if(p[i].y < mmin[1])
            mmin[1] = p[i].y;
        if(p[i].y > mmax[1])
            mmax[1] = p[i].y;
        //z
        if(p[i].z < mmin[2])
            mmin[2] = p[i].z;
        if(p[i].z > mmax[2])
            mmax[2] = p[i].z;
    }
    std::shared_ptr<AABB>res(new AABB(mmin[0], mmax[0], mmin[1], mmax[1], mmin[2], mmax[2]));
    return res;
}
