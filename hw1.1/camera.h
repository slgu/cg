#pragma once
#include "point.h"
#include "vect.h"
#include "helper.h"
#include "ray.h"
class camera {
public:
    point eye;
    float d;
    vect u;
    vect v;
    vect w;
    int nx;
    int ny;
    float l, r, t, b;
    //float x, y, z, vx, vy, vz, d, iw, ih, pw, ph;
    camera(float x, float y, float z, float vx, float vy, float vz, float _d, float iw, float ih, float pw, float ph)
        :eye(x, y ,z), w(-vx, -vy, -vz), d(_d){
        vect dir(vx, vy, vz);
        vect tmp(0, 1, 0);
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
    ray generate_ray(int i, int j) {
        float _u = l + (r - l) * (i + 0.5) / nx;
        float _v = b + (t - b) * (j + 0.5) / ny;
        vect dir = _u * u + _v * v - d * w;
        norm(dir);
        return ray(eye, dir);
    }
};
