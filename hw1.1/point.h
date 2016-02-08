#pragma once
class point {
    float x;
    float y;
    float z;
public:
    point(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    point() {
        x = 0;
        y = 0;
        z = 0;
    }
};
