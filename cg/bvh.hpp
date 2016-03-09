//
//  bvh.hpp
//  cg
//
//  Created by GuShenlong on 3/7/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#ifndef bvh_hpp
#define bvh_hpp

#include <stdio.h>
#include "surface.h"
#include <vector>
#include <algorithm>
class bvh_node {
public:
    std::shared_ptr <AABB> ab_node;
    std::shared_ptr <bvh_node> left;
    std::shared_ptr <bvh_node> right;
    //construct a bvh tree from existing surfaces
    static void cal_bounding_box(std::vector <std::shared_ptr<AABB>> & boxes, int i, int j, std::shared_ptr <AABB> & box) {
        for (int o = i; o < j; ++o) {
            for (int k = 0; k < 3; ++k) {
                if (boxes[o]->mmin[k] < box->mmin[k])
                    box->mmin[k] = boxes[o]->mmin[k];
                if (boxes[o]->mmax[k] > box->mmax[k])
                    box->mmax[k] = boxes[o]->mmax[k];
            }
        }
    }
    static void bvhsort(std::vector <std::shared_ptr<AABB>> &boxes, int i);
    static std::shared_ptr <bvh_node> build(std::vector <std::shared_ptr<AABB>> &boxes);
    static void intersect_obj(std::shared_ptr<bvh_node> node, ray & ry, float & t0, std::shared_ptr<AABB> & nearest_aabb, int type);
    static void intersect_box(std::shared_ptr<bvh_node> node, ray & ry, float & t0, std::shared_ptr<AABB> & nearest_aabb, int type);
    static void debug(std::shared_ptr<bvh_node> node) {
        if (node != nullptr) {
            std::cout << "this box" << std::endl;
            for (int i = 0; i < 3; ++i)
                printf("%.2f ", node->ab_node->mmin[i]);
            std::cout << std::endl;
            for (int i = 0; i < 3; ++i)
                printf("%.2f ", node->ab_node->mmax[i]);
            std::cout << std::endl;
            std::cout << "left" << std::endl;
            debug(node->left);
            std::cout << "right" << std::endl;
            debug(node->right);
            std::cout << "this box end" << std::endl;
            
        }
    }
};
#endif /* bvh_hpp */
