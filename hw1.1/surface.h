#pragma once
#include "helper.h"
#include "ray.h"
class surface {
    virtual bool intersect(ray & r) = 0;
};
