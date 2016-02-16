#pragma once
#include "surface.h"
#include "vector.h"
class plane : public surface {
    vector normal;
    float d;
public:
    plane() {

    }
    plane(float nx, float ny, float nz, float _d) {
        normal = vector(nx, ny, nz);
        norm(normal);
        d = _d
    }
};
