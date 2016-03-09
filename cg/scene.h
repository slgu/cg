#pragma once
#include <memory>
#include <vector>
#include "surface.h"
#include "exr.h"
#include "camera.h"
#include "light.hpp"
#include "bvh.hpp"
class Scene {
    //camera
public:
    const float SHADOW_COE;
    std::shared_ptr <bvh_node> bvh_tree;
    int cmd;
    shared_ptr <camera> c;
    //multiple lights
    std::vector < shared_ptr <light> > lights;
    //multiple objs
    std::vector < shared_ptr <surface> > objs;
    std::vector < shared_ptr <AABB> > boxes;
    //init
    Scene():SHADOW_COE(0.001){
        bvh_tree = nullptr;
    }
    //get intersection and save the result
    void get_intersection(std::string filename);
    bool trace(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
    bool trace_normal(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
    bool trace_aabb(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
    bool trace_bvh(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
    bool trace_bvh_aabb(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type);
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
    void set_cmd(int __cmd) {
        cmd = __cmd;
    }
    void pre() {
        if (cmd == 0) {
        
        }
        else {
            //create boxes for each surface
            int l = int(objs.size());
            for (int i = 0; i < l; ++i) {
                std::shared_ptr <AABB> box = objs[i]->get_aabb();
                box->set_obj(objs[i]);
                boxes.push_back(box);
            }
            if (cmd == 1) {
                
            }
            else {
                bvh_tree = bvh_node::build(boxes);
            }
        }
    }
    Rgba recur_ray_cal(ray & ry, int depth);
};
