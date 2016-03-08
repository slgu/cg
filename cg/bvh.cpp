//
//  bvh.cpp
//  cg
//
//  Created by GuShenlong on 3/7/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include "bvh.hpp"
bool cmpx(const std::shared_ptr<AABB> & a1, const std::shared_ptr<AABB> & a2) {
    return a1->mmin[0] < a2->mmin[0];
}
bool cmpy(const std::shared_ptr<AABB> & a1, const std::shared_ptr<AABB> & a2) {
    return a1->mmin[1] < a2->mmin[1];
}
bool cmpz(const std::shared_ptr<AABB> & a1, const std::shared_ptr<AABB> & a2) {
    return a1->mmin[2] < a2->mmin[2];
}

void bvh_node::bvhsort(std::vector <std::shared_ptr<AABB>> &boxes, int i) {
    if (i == 0)
        std::sort(boxes.begin(), boxes.end(), cmpx);
    else if (i == 1)
        std::sort(boxes.begin(), boxes.end(), cmpy);
    else
        std::sort(boxes.begin(), boxes.end(), cmpz);
}

//bvh tree for speeding up intersection with aabb
std::shared_ptr <bvh_node> bvh_node::build(std::vector <std::shared_ptr<AABB>> &boxes) {
    int l = int(boxes.size());
    if (l == 0)
        return nullptr;
    if (l == 1) {
        std::shared_ptr<bvh_node> res(new bvh_node());
        //leaft node, we attach our boxes with data into it
        res->ab_node = boxes[0];
        res->left = res->right = nullptr;
        return res;
    }
    std::shared_ptr <bvh_node> res_node(new bvh_node());
    std::shared_ptr <AABB> ab_node(new AABB());
    std::shared_ptr <AABB> tmp_node(new AABB());
    cal_bounding_box(boxes, 0, l, ab_node);
    //set ab_node
    res_node->ab_node = ab_node;
    //select axis
    int half = l / 2;
    float best_volume = -1;
    int best_idx = -1;
    for (int i = 0; i < 3; ++i) {
        bvhsort(boxes, i);
        float volume = 0;
        cal_bounding_box(boxes, 0, half, tmp_node);
        volume += tmp_node->get_volume();
        cal_bounding_box(boxes, half, l, tmp_node);
        volume += tmp_node->get_volume();
        if (best_volume == -1 || volume < best_volume) {
            best_volume = volume;
            best_idx = i;
        }
    }
    bvhsort(boxes, best_idx);
    
    //recursive call left and right
    std::vector <std::shared_ptr<AABB>> left_boxes;
    std::vector <std::shared_ptr<AABB>> right_boxes;
    left_boxes.assign(boxes.begin(), boxes.begin() + half);
    right_boxes.assign(boxes.begin() + half, boxes.end());
    res_node->left = build(left_boxes);
    res_node->right = build(right_boxes);
    return res_node;
}