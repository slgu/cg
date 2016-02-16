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
    shared_ptr <camera> c;
    //multiple lights
    std::vector < shared_ptr <light> > lights;
    //multiple objs
    std::vector < shared_ptr <surface> > objs;
    void get_intersection(std::string filename) {
        Array2D <Rgba> pixels;
        pixels.resizeErase(c->ny, c->nx);
        for (int i = 0; i < c->nx; ++i)
            for (int j = 0; j < c->ny; ++j) {
                //generate ray
                ray ry = c->generate_ray(i, j);
                Rgba & rgb = pixels[j][i];
                rgb.r = rgb.b = rgb.g = 0;
                bool flg = false;
                float t0 = -1;
                shared_ptr <surface> nearest_surface;
                for (auto itr = objs.begin(); itr != objs.end(); ++itr) {
                    float tmp;
                    if ((*itr)->intersect(ry, tmp)) {
                        flg = true;
                        if (t0 == -1 || t0 > tmp) {
                            t0 = tmp;
                            nearest_surface = *itr;
                        }
                    }
                }
                if (!flg) {
                    //nothing to do if no intersection
                    continue;
                }
                //calculate result
                float r = 0;
                float g = 0;
                float b = 0;
                //get intersect p
                point intersect_p = ry.get_t(t0);
                //get material
                material & m = (*nearest_surface->m);
                vect inter = ry.dir;
                //faxiangliang
                vect n = nearest_surface->get_n(intersect_p);
                for (auto light = lights.begin(); light != lights.end(); ++light) {
                    float tmp_r, tmp_g, tmp_b;
                    (*light)->calculate_rgb(intersect_p, n, m, inter, tmp_r, tmp_g, tmp_b);
                    //light shadow check TODO
                    r += tmp_r;
                    g += tmp_g;
                    b += tmp_b;
                }
                //set pixels
            }
        write_exr_file(filename, &pixels[0][0], c->nx, c->ny);
    }
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
};
