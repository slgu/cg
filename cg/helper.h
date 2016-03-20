#pragma once
#include "vect.h"
#include "point.h"
#include <cmath>
#include <random>
inline int norm(vect &d) {
    float sum = d.x * d.x + d.y * d.y + d.z * d.z;
    sum = sqrt(sum);
    d.x = d.x / sum;
    d.y = d.y / sum;
    d.z = d.z / sum;
    return sum;
}

inline vect cross_product(const vect & v1, const vect & v2) {
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vect(x, y, z);
}

inline float inner_product(const vect & v1, const vect & v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float inner_product(const point & v1, const vect & v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline bool check_eps(float a) {
    if (a < 0) a = -a;
    if (a < 1e-3)
        return true;
    else
        return false;
}

template <typename T> void swap(T & a, T & b) {
    T c = a;
    a = b;
    b = c;
}

class global_random {
private:
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution;
    global_random():distribution(0.0, 0.1) {
        
    }
public:
    static std::shared_ptr <global_random> single() {
        static std::shared_ptr <global_random> ptr;
        if (ptr == nullptr) {
            ptr.reset(new global_random());
        }
        return ptr;
    }
    double next() {
        return distribution(generator);
    }
};

#define SHADOW_RAY 1
#define NORMAL_RAY 0