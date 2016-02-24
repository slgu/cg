//
//  intersection.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "intersection.h"
//#define INTER_DEBUG
void intersection::get_intersection(std::string filename) {
    Array2D <Rgba> pixels;
    pixels.resizeErase(c->ny, c->nx);
    for (int i = 0; i < c->nx; ++i)
        for (int j = 0; j < c->ny; ++j) {
#ifdef INTER_DEBUG
            std::cout << "now:" << 100.0 * (i * c->ny + j) / c->nx / c->ny << std::endl;
#endif
            //generate ray
            ray ry = c->generate_ray(i, j);
            Rgba & rgb = pixels[c-> ny - 1 - j][i];
            rgb.r = rgb.b = rgb.g = 0;
            shared_ptr<surface> nearest_surface = nullptr;
            //no intersection
            float t0;
            if (!trace(ry, t0, nearest_surface, NORMAL_RAY))
                continue;
            //calculate result
            float r = 0;
            float g = 0;
            float b = 0;
            //get intersect p
            point intersect_p = ry.get_t(t0);
            //get material
            material & m = (*nearest_surface->m);
            vect v = -ry.dir;
            //faxiangliang
            vect n = nearest_surface->get_n(intersect_p);
            //add all lights contribution together
            for (auto light = lights.begin(); light != lights.end(); ++light) {
                float tmp_r = 0, tmp_g = 0, tmp_b = 0;
                //if ambient no check for shadow ray
                if ((*light) -> check_ambient());
                else {
                    //check shadow ray
                    ray shadow_ray = (*light)->generate_shadow_ray(ry.get_t(t0 * (1- SHADOW_COE)), t0);
                    if (trace(shadow_ray, t0, nearest_surface, SHADOW_RAY))
                        //if intersect
                        continue;
                }
                (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
                r += tmp_r;
                g += tmp_g;
                b += tmp_b;
            }
            rgb.r = r;
            rgb.g = g;
            rgb.b = b;
            //set pixels
        }
    write_exr_file(filename, &pixels[0][0], c->nx, c->ny);
}

//just trace the objs to see if intersection
bool intersection::trace(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    bool flg = false;
    if (type != SHADOW_RAY) {
        t0 = -1;
    }
    for (auto itr = objs.begin(); itr != objs.end(); ++itr) {
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
