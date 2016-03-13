//
//  intersection.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "scene.h"
#define INTER_DEBUG
void Scene::get_intersection(std::string filename) {
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
            Rgba res = recur_ray_cal(ry, 5);
            rgb.r = res.r;
            rgb.g = res.g;
            rgb.b = res.b;
        }
    write_exr_file(filename, &pixels[0][0], c->nx, c->ny);
}

bool Scene::trace_normal(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    bool flg = false;
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

bool Scene::trace_aabb(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    bool flg = false;
    for (auto itr = boxes.begin(); itr != boxes.end(); ++itr) {
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

bool Scene::trace_bvh(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    return bvh_node::intersect_obj(bvh_tree, ry, t0, nearest_surface, type);
}

bool Scene::trace_bvh_aabb(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    std::shared_ptr <AABB> res = nullptr;
    bool flg = bvh_node::intersect_box(bvh_tree, ry, t0, res, type);
    nearest_surface = res;
    return flg;
}

//just trace the objs to see if intersection
bool Scene::trace(ray & ry, float & t0, shared_ptr<surface> & nearest_surface, int type) {
    if (type != SHADOW_RAY)
        t0 = - 1;
    bool flg1 = false;
    bool flg2 = false;
    if (cmd == 0) {
        flg1 = trace_normal(ry, t0, nearest_surface, type);
        if (type == SHADOW_RAY && flg1)
            return true;
        flg2 = trace_plane(ry, t0, nearest_surface, type);
        return flg1 || flg2;
    }
    else if (cmd == 1) {
        flg1 = trace_aabb(ry, t0, nearest_surface, type);
        if (type == SHADOW_RAY && flg1)
            return true;
        flg2 = trace_plane(ry, t0, nearest_surface, type);
        return flg1 || flg2;
    }
    else if (cmd == 2) {
        flg1 = trace_bvh_aabb(ry, t0, nearest_surface, type);
        if (type == SHADOW_RAY && flg1)
            return true;
        flg2 = trace_plane(ry, t0, nearest_surface, type);
        return flg1 || flg2;
    }
    else if (cmd == 3) {
        flg1 = trace_bvh(ry, t0, nearest_surface, type);
        if (type == SHADOW_RAY && flg1)
            return true;
        flg2 = trace_plane(ry, t0, nearest_surface, type);
        return flg1 || flg2;
    }
    else
        return false;
}

Rgba Scene::recur_ray_cal(ray & ry, int depth) {
    shared_ptr<surface> nearest_surface = nullptr;
    
    //if depth = 0 return
    if (depth == 0) {
        Rgba res;
        //just no intersect
        res.r = res.g = res.b = 0;
        return res;
    }
    
    //no intersection
    float t0;
    float shadow_check_t;
    if (!trace(ry, t0, nearest_surface, NORMAL_RAY)) {
        Rgba res;
        //just no intersect
        res.r = res.g = res.b = 0;
        return res;
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
    norm(n);
    //add all lights contribution together
    for (auto light = lights.begin(); light != lights.end(); ++light) {
        float tmp_r = 0, tmp_g = 0, tmp_b = 0;
        //if ambient no check for shadow ray
        if ((*light) -> check_ambient());
        else {
            //check shadow ray
            ray shadow_ray = (*light)->generate_shadow_ray(ry.get_t(t0 * (1- SHADOW_COE)), shadow_check_t);
            if (trace(shadow_ray, shadow_check_t, nearest_surface, SHADOW_RAY))
                //if intersect
                continue;
        }
        (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
        r += tmp_r;
        g += tmp_g;
        b += tmp_b;
    }
    //if [0,0,0] no need for recur ray trace
    if(m.ir == 0 && m.ig == 0 && m.ib == 0)
        return Rgba(r, g, b);
    //use reflective ray to recur ray tray
    vect reflect_ray_dir = ry.dir - 2 * inner_product(ry.dir, n) * n;
    norm(reflect_ray_dir);
    ray reflect_ray(ry.get_t(t0 * (1- SHADOW_COE)), reflect_ray_dir);
    Rgba res = recur_ray_cal(reflect_ray, depth - 1);
    float coe = inner_product(reflect_ray_dir, n);
    coe = 1;//N * r needed or not ?
    res.r = r + m.ir * coe * res.r;
    res.g = g + m.ig * coe * res.g;
    res.b = b + m.ib * coe * res.b;
    return res;
}
