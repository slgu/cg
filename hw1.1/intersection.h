#pragma once
#include <memory>
#include <vector>
#include "surface.h"
#include "exr.h"
class intersection {
    //camera
public:
    shared_ptr <camera> c;
    std::vector < shared_ptr <surface> > objs;
    void get_intersection(std::string filename) {
        Array2D <Rgba> pixels;
        pixels.resizeErase(c->ny, c->nx);
        for (int i = 0; i < c->nx; ++i)
            for (int j = 0; j < c->ny; ++j) {
                //generate ray
                bool flg = false;
                ray r = c->generate_ray(i, j);
                Rgba & rgb = pixels[j][i];
                rgb.r = rgb.b = rgb.g = 0;
                for (auto itr = objs.begin(); itr != objs.end(); ++itr) {
                    if ((*itr)->intersect(r)) {
                        //set red
                        rgb.r = 1;
                    }
                }
            }
        write_exr_file(filename, &pixels[0][0], c->nx, c->ny);
    }
    void set_camera(camera * _c) {
        c.reset(_c);
    }
    void add_obj(surface * obj) {
        shared_ptr <surface> tmp(obj);
        objs.push_back(tmp);
    }
};
