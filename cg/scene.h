#pragma once
#include <memory>
#include <vector>
#include "surface.h"
#include "exr.h"
#include "camera.h"
#include "light.hpp"
#include "bvh.hpp"
#include <vector>
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
    //surfaces
    std::vector < shared_ptr <plane> > planes;
    std::vector < shared_ptr <AABB> > boxes;
    std::vector <std::vector <Rgba>> pixels;
    //init
    int pri_ray_num;
    int shadow_ray_num;
    Scene():SHADOW_COE(0.001){
        bvh_tree = nullptr;
    }
    //get intersection and save the result
    void render();
    void write_to_file(std::string filename) {
        Array2D <Rgba> file_pixels;
        file_pixels.resizeErase(c->ny, c->nx);
        for (int i = 0; i < c->ny; ++i)
            for (int j = 0; j < c->nx; ++j) {
                file_pixels[i][j].r = pixels[i][j].r;
                file_pixels[i][j].g = pixels[i][j].g;
                file_pixels[i][j].b = pixels[i][j].b;
            }
        write_exr_file(filename, &file_pixels[0][0], c->nx, c->ny);
    }
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
    //add plane
    void add_plane(plane * p) {
        shared_ptr <plane> tmp(p);
        planes.push_back(tmp);
    }
    void set_cmd(int __cmd) {
        cmd = __cmd;
    }
    void set_pri_ray_num(int _num) {
        pri_ray_num = _num;
    }
    void set_shadow_ray_num(int _num) {
        shadow_ray_num = _num;
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
            //debug for boxes
            /*
            for (auto itr = boxes.begin(); itr != boxes.end(); ++itr)
                (*itr)->debug_aabb();
             */
        }
    }
    bool trace_plane(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
        bool flg = false;
        for (auto itr = planes.begin(); itr != planes.end(); ++itr) {
            float tmp;
            if ((*itr)->intersect(ry, tmp)) {
                //if shadow no need nearest search
                if (type == SHADOW_RAY) {
                    if (t0 > tmp)
                        return true;
                }
                else {
                    flg = true;
                    if (t0 == -1 || t0 > tmp) {
                        t0 = tmp;
                        nearest_surface = *itr;
                    }
                }
            }
        }
        if (!flg) {
            return false;
        }
        else {
            return true;
        }
    }
    Rgba recur_ray_cal(ray & ry, int depth);
};
