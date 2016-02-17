#pragma once
class vect {
public:
    float x;
    float y;
    float z;
    vect(float _x, float _y, float _z) {
        x = _x;
        y = _y;
        z = _z;
    }
    vect(){
        x = 0;
        y = 0;
        z = 0;
    }
    friend vect operator-(const vect & o1, const vect & o2) {
        return vect(o1.x - o2.x, o1.y - o2.y, o1.z - o2.z);
    }
    friend vect operator+(const vect & o1, const vect & o2) {
        return vect(o1.x + o2.x, o1.y + o2.y, o1.z + o2.z);
    }
    friend vect operator*(const vect & o1, float a) {
        return vect(a * o1.x, a * o1.y, a * o1.z);
    }
    friend vect operator*(float a, const vect & o1) {
        return vect(a * o1.x, a * o1.y, a * o1.z);
    }
    vect operator-() const{
        return vect(-x,-y,-z);
    }
};
