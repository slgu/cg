//
//  light.cpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 . All rights reserved.
//

#include "light.hpp"
#include "helper.h"

void plight::calculate_rgb(point & intersect_p, vect & n, material & m,
                           vect & inter, float & r, float & g, float & b) {
    //diffuse
    vect l = p - intersect_p;
    norm(l);
    norm(n);
    norm(inter);
    float extent = inner_product(l, n);
    if (extent < 0) extent = 0;
    r = extent * color[0] * m.dr;
    g = extent * color[1] * m.dg;
    b = extent * color[2] * m.db;
    //specular
    vect h = l + inter;
    norm(h);
    extent = inner_product(n, h);
    if (extent < 0) extent = 0;
    //set exponent extent
    extent = pow(extent, m.r);
    r += extent * color[0] * m.sr;
    g += extent * color[1] * m.sg;
    b += extent * color[2] * m.sb;
}

void alight::calculate_rgb(point & intersect_p, vect & n, material & m,
                           vect & inter, float & r, float & g, float & b) {
    
    r = m.dr * color[0];
    g = m.dg * color[1];
    b = m.db * color[2];
}

bool alight::check_ambient() {
    return true;
}

bool plight::check_ambient() {
    return false;
}

ray alight::generate_shadow_ray(const point & inter_p, float & t) {
    return ray(point(0,0,0), vect(0,0,0));
}

ray plight::generate_shadow_ray(const point & inter_p, float & t) {
    vect dir = p - inter_p;
    t = norm(dir);
    return ray(inter_p, dir);
}