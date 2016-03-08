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
int main (int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "useage: ./prog_out scenefilename outputopenexr" << std::endl;
        return -1;
    }
    std::shared_ptr <Scene> scene(new Scene());
    Parser parser;
    parser.parse_scene_file(argv[1], scene);
    scene->get_intersection(argv[2]);
    return 0;
}