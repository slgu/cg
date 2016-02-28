#pragma once
#include <memory>
#include <vector>
#include "surface.h"
#include "exr.h"
#include "camera.h"
#include "light.hpp"
class intersection {
    //camera
public:
    static const int SHADOW_RAY = 1;
    static const int NORMAL_RAY = 0;
    const float SHADOW_COE;
    shared_ptr <camera> c;
    //multiple lights
    std::vector < shared_ptr <light> > lights;
    //multiple objs
    std::vector < shared_ptr <surface> > objs;
    //init
    intersection():SHADOW_COE(0.001){
        
    }
    //get intersection and save the result
    void get_intersection(std::string filename);
    bool trace(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
    //set camera
    void set_camera(camera * _c) {
        c.reset(_c);
    }
    //add obj
    void add_obj(surface * obj) {
        shared_ptr <surface> tmp(obj);
        objs.push_back(tmp);
    }
    //add light
    void add_light(light * l) {
        shared_ptr <light> tmp(l);
        lights.push_back(tmp);
    }
    Rgba recur_ray_cal(ray & ry, int depth);
};
