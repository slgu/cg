//
//  main.cpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#include <iostream>
#include "parse.h"
#include "intersection.h"
int main (int argc, char *argv[]) {
    /*
    if (argc != 3) {
        std::cerr << "useage: ./raytra scenefilename outputopenexr" << std::endl;
        return -1;
    }
    */
    std::shared_ptr <intersection> scene(new intersection());
    parseSceneFile ("/Users/slgu1/Desktop/test", scene);
    scene->get_intersection("/Users/slgu1/Desktop/test.exr");
    return 0;
}