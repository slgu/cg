//
//  intersection.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "intersection.h"

void intersection::get_intersection(std::string filename) {
    Array2D <Rgba> pixels;
    pixels.resizeErase(c->ny, c->nx);
    for (int i = 0; i < c->nx; ++i)
        for (int j = 0; j < c->ny; ++j) {
            //generate ray
            ray ry = c->generate_ray(i, j);
            Rgba & rgb = pixels[c-> ny - 1 - j][i];
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
            vect v = -ry.dir;
            //faxiangliang
            vect n = nearest_surface->get_n(intersect_p);
            for (auto light = lights.begin(); light != lights.end(); ++light) {
                float tmp_r = 0, tmp_g = 0, tmp_b = 0;
                (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
                //light shadow check TODO
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
