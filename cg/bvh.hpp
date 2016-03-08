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
                if (boxes[i]->mmin[k] < box->mmin[k])
                    box->mmin[k] = boxes[i]->mmin[k];
                if (boxes[i]->mmax[k] > box->mmin[k])
                    box->mmax[k] = boxes[i]->mmax[k];
            }
        }
    }
    static void bvhsort(std::vector <std::shared_ptr<AABB>> &boxes, int i);
    static std::shared_ptr <bvh_node> build(std::vector <std::shared_ptr<AABB>> &boxes);
};
#endif /* bvh_hpp */
