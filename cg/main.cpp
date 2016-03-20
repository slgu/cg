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
#define MAIN_DEBUG
int main (int argc, char *argv[]) {
#ifndef MAIN_DEBUG
    if (argc != 4) {
        std::cerr << "useage: ./prog_out scenefilename outputopenexr param" << std::endl;
        return -1;
    }
    int cmd = atoi(argv[3]);
    std::shared_ptr <Scene> scene(new Scene());
    Parser parser;
    parser.parse_scene_file(argv[1], scene);
    scene->set_cmd(cmd);
    scene->pre();
    scene->get_intersection(argv[2]);
#else
    std::shared_ptr <Scene> scene(new Scene());
    Parser parser;
    parser.parse_scene_file("/Users/slgu1/Desktop/test", scene);
    scene->set_cmd(3);
    scene->set_pri_ray_num(3);
    scene->set_shadow_ray_num(5);
    scene->pre();
    scene->get_intersection("/Users/slgu1/Desktop/test.exr");
#endif
    return 0;
}