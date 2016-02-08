#pragma once
#include <memory>
#include <vector>
#include "surface.h"
class intersection {
    //camera
public:
    shared_ptr <camera> c;
    std::vector < shared_ptr <surface> > objs;
    void get_intersection(std::string filename) {

        for (auto itr = objs.begin(); itr != objs.end(); ++itr) {

        }
    }
};
