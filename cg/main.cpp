//
//  main.cpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <iostream>
#include "parse.h"
#include "scene.h"
//#define MAIN_DEBUG
int main (int argc, char *argv[]) {
#ifndef MAIN_DEBUG
    if (argc != 5) {
        std::cerr << "useage: ./prog_out scenefilename outputopenexr pri_num shadow_num" << std::endl;
        return -1;
    }
    int pri_num = atoi(argv[3]);
    int shadow_num = atoi(argv[4]);
    std::shared_ptr <Scene> scene(new Scene());
    Parser parser;
    parser.parse_scene_file(argv[1], scene);
    scene->set_cmd(3); //default using bvh and bounding box
    scene->set_pri_ray_num(pri_num);
    scene->set_shadow_ray_num(shadow_num);
    scene->pre();
    scene->render();
    scene->write_to_file(argv[2]);
#else
    std::shared_ptr <Scene> scene(new Scene());
    Parser parser;
    parser.parse_scene_file("/Users/slgu1/Dropbox/graduate_courses@CU/cg/cg/test/test14", scene);
    scene->set_cmd(3);
    scene->set_pri_ray_num(2);
    scene->set_shadow_ray_num(2);
    scene->pre();
    scene->render();
    scene->write_to_file("/Users/slgu1/Dropbox/graduate_courses@CU/cg/cg/test/test14.exr");
#endif
    return 0;
}