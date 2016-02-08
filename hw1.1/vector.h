#pragma once
class vector {
public:
    float x;
    float y;
    float z;
    vector(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    vector(){
        x = 0;
        y = 0;
        z = 0;
    }
};
