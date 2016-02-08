#pragma once
#include "helper.h"
#include "ray.h"
class surface {
public:
    virtual bool intersect(ray & r) = 0;
};
