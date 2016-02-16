//
//  material.hpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 slgu. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include <stdio.h>
class material {
public:
    float dr;
    float dg;
    float db;
    float sr;
    float sg;
    float sb;
    float ir;
    float ig;
    float ib;
    material(float _dr, float _dg, float _db, float _sr, float _sg, float _sb, float _ir, float _ig, float _ib)
        :dr(_dr), dg(_dg), db(_db), sr(_sr), sg(_sg), sb(_sb), ir(_ir), ig(_ig), ib(_ib) {
        
    }
};
#endif /* material_hpp */
