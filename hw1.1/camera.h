#pragma once
#include "point.h"
#include "vector.h"
#include "helper.h"
class camera {
    point eye;
    float d;
    vector u;
    vector v;
    vector w;
    int nx;
    int ny;
    float l, r, t, b;
public:
    //float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
    camera(float x, float y, float z, float vx, float vy, float vz, float _d, float iw, float ih, float pw, float ph)
        :eye(x, y ,z), w(-vx, -vy, -vz), d(_d){
        vector dir(vx, vy, vz);
        vector tmp(0, 1, 0);
        u = cross_product(dir, tmp);
        norm(u);
        v = cross_product(u, dir);
        norm(v);
        norm(w);
        nx = int(pw);
        ny = int(ph);
        r = iw / 2;
        l = -r;
        t = ih / 2;
        b = -t;
    }
};
