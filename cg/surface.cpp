//
//  surface.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "surface.h"
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
    std::cout << "intersect in plane" << std::endl;
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