//
//  intersection.cpp
//  cg
//
//  Created by GuShenlong on 2/16/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <stdio.h>
#include "scene.h"
#include <random>
#include "helper.h"
//#define INTER_DEBUG
//#define USE_TBB
//#define USE_GLOSSY
#ifdef USE_TBB
#include "tbb/parallel_for.h"
#endif
void Scene::render() {
    pixels.clear();
    for (int i = 0; i < c->ny; ++i) {
        pixels.push_back(vector<Rgba>());
    }
    for (int i = 0; i < c->ny; ++i)
        for (int j = 0; j < c->nx; ++j)
            pixels[i].push_back(Rgba());
    //sample pri_ray_num ^ 2 ray
#ifdef USE_TBB
    tbb::parallel_for(0, c->nx, 1, [=] (int i) {
        tbb::parallel_for(0, c->ny, 1, [=] (int j) {
#else
    for (int i = 0; i < c->nx; ++i)
        for (int j = 0; j < c->ny; ++j) {
#endif
#ifdef INTER_DEBUG
            std::cout << "now:" << 100.0 * (i * c->ny + j) / c->nx / c->ny << std::endl;
#endif
            //generate ray
            Rgba rgb;
            rgb.r = rgb.b = rgb.g = 0;
            for (int k = 0; k < pri_ray_num; ++k)
                for(int o = 0; o < pri_ray_num; ++o) {
                    double rand_num= global_random::single()->next();
                    ray ry = c->generate_ray(i + (k + rand_num) / pri_ray_num, j +
                                             (o + rand_num) / pri_ray_num);
                    Rgba res = recur_ray_cal(ry, 10);
                    rgb.r += res.r;
                    rgb.g += res.g;
                    rgb.b += res.b;
                }
            rgb.r /= (pri_ray_num * pri_ray_num);
            rgb.g /= (pri_ray_num * pri_ray_num);
            rgb.b /= (pri_ray_num * pri_ray_num);
            pixels[c-> ny - 1 - j][i].r = rgb.r;
            pixels[c-> ny - 1 - j][i].g = rgb.g;
            pixels[c-> ny - 1 - j][i].b = rgb.b;
        }
#ifdef USE_TBB
        );});
#endif
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
        if ((*light) -> check_ambient()) {
            (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
            r += tmp_r;
            g += tmp_g;
            b += tmp_b;
        }
        else if ((*light) -> check_area()) {
            //check shadow ray
            //sample shadow_ray_num * 2 shadow ray and sum average
            float shadow_avg_r = 0;
            float shadow_avg_g = 0;
            float shadow_avg_b = 0;
            //stratify
            for (int o = 0; o < shadow_ray_num; ++o)
                for (int k = 0; k < shadow_ray_num; ++k) {
                    ray shadow_ray = (std::dynamic_pointer_cast<slight>(*light))->generate_area_shadow_ray(ry.get_t(t0 * (1- SHADOW_COE)), shadow_ray_num, o, k, shadow_check_t);
                    if (trace(shadow_ray, shadow_check_t, nearest_surface, SHADOW_RAY))
                        //if intersect
                        continue;
                    (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
                    shadow_avg_r += tmp_r;
                    shadow_avg_g += tmp_g;
                    shadow_avg_b += tmp_b;
                }
            shadow_avg_r /= (shadow_ray_num * shadow_ray_num);
            shadow_avg_g /= (shadow_ray_num * shadow_ray_num);
            shadow_avg_b /= (shadow_ray_num * shadow_ray_num);
            r += shadow_avg_r;
            g += shadow_avg_g;
            b += shadow_avg_b;
        }
        else {
            ray shadow_ray = (*light)->generate_shadow_ray(ry.get_t(t0 * (1- SHADOW_COE)), shadow_check_t);
            if (trace(shadow_ray, shadow_check_t, nearest_surface, SHADOW_RAY))
                //if intersect
                continue;
            (*light)->calculate_rgb(intersect_p, n, m, v, tmp_r, tmp_g, tmp_b);
            r += tmp_r;
            g += tmp_g;
            b += tmp_b;
        }
    }
    //if [0,0,0] no need for recur ray trace
    if(m.ir == 0 && m.ig == 0 && m.ib == 0)
        return Rgba(r, g, b);
    
    //add glossy reflection TODO needed
    //use reflective ray to recur ray tray
    vect reflect_ray_dir = ry.dir - 2 * inner_product(ry.dir, n) * n;
    norm(reflect_ray_dir);
#ifdef USE_GLOSSY
#define GLOSSY_WIDTH 0.3
#define GLOSSY_NUM 3
    vect ru_dir(reflect_ray_dir.z, 0, -reflect_ray_dir.x);
    vect rv_dir = cross_product(reflect_ray_dir, ru_dir);
    norm(ru_dir);
    norm(rv_dir);
    Rgba res;
    res.r = res.g = res.b = 0;
    for (int o = 0; o < GLOSSY_NUM; ++o)
        for (int k = 0; k < GLOSSY_NUM; ++k) {
            double ru = global_random::single()->next();
            double rv = global_random::single()->next();
            vect random_reflect_dir = reflect_ray_dir + (ru - 0.5) * GLOSSY_WIDTH * ru_dir + (rv - 0.5) * GLOSSY_WIDTH * rv_dir;
            norm(random_reflect_dir);
            ray random_reflect_ray(ry.get_t(t0 * (1- SHADOW_COE)), random_reflect_dir);
            Rgba tmp_res = recur_ray_cal(random_reflect_ray, depth - 1);
            res.r += tmp_res.r;
            res.g += tmp_res.g;
            res.b += tmp_res.b;
        }
    res.r /= (GLOSSY_NUM * GLOSSY_NUM);
    res.g /= (GLOSSY_NUM * GLOSSY_NUM);
    res.b /= (GLOSSY_NUM * GLOSSY_NUM);
    res.r = r + m.ir * res.r;
    res.g = g + m.ig * res.g;
    res.b = b + m.ib * res.b;
    return res;
#else
    ray reflect_ray(ry.get_t(t0 * (1- SHADOW_COE)), reflect_ray_dir);
    Rgba res = recur_ray_cal(reflect_ray, depth - 1);
    res.r = r + m.ir * res.r;
    res.g = g + m.ig * res.g;
    res.b = b + m.ib * res.b;
    return res;
#endif
}
