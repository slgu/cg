//
//  parse.cpp
//  cg
//
//  Created by GuShenlong on 2/15/16.
//  Copyright © 2016 slgu. All rights reserved.
//
#include <cmath>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;
#include "camera.h"
#include "intersection.h"
#include "surface.h"
#include "sphere.h"
#include "material.hpp"

//read float from input stream
float getTokenAsFloat (string inString, int whichToken)
{
    
    float thisFloatVal = 0.;    // the return value
    
    if (whichToken == 0) {
        cerr << "error: the first token on a line is a character!" << endl;
        exit (-1);
    }
    
    // c++ string class has no super-easy way to tokenize, let's use c's:
    char *cstr = new char [inString.size () + 1];
    
    strcpy (cstr, inString.c_str());
    
    char *p = strtok (cstr, " ");
    if (p == 0) {
        cerr << "error: the line has nothing on it!" << endl;
        exit (-1);
    }
    
    for (int i = 0; i < whichToken; i++) {
        p = strtok (0, " ");
        if (p == 0) {
            cerr << "error: the line is not long enough for your token request!" << endl;
            exit (-1);
        }
    }
    
    thisFloatVal = atof (p);
    
    delete[] cstr;
    
    return thisFloatVal;
}


//
// read the scene file.
//
//
void parseSceneFile (char *filnam, shared_ptr <intersection> & scene)
{
    
    ifstream inFile(filnam);    // open the file
    string line;
    
    if (! inFile.is_open ()) {  // if it's not open, error out.
        cerr << "can't open scene file" << endl;
        exit (-1);
    }
    
    
    // Note: you'll have to keep track of whatever the last material
    // you loaded in was, so you can apply it to any geometry that gets loaded.
    // So here, you'll have something like:
    //
    // myMaterialClass *lastMaterialLoaded = 0;  // 0 or maybe a default material?
    //
    // and each time you load in a new piece of geometry (sphere, triangle, plane)
    // you will set its material to lastMaterialLoaded.
    
    
    std::shared_ptr <material> material_ptr;
    while (! inFile.eof ()) {   // go through every line in the file until finished
        
        getline (inFile, line); // get the line
        sphere * sphere_ptr;
        camera * camera_ptr;
        plight * plight_ptr;
        switch (line[0])  {     // we'll decide which command based on the first character
                
                //
                // geometry types:
                //
                // NOTE: whichever type of geo you load in, set its material to
                // be "lastMaterialLoaded"
                //
            case 's':
                // it's a sphere, load in the parameters
                
                float x, y, z, r;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);
                r = getTokenAsFloat (line, 4);
                sphere_ptr = new sphere(x, y, z, r);
                //set material
                sphere_ptr->set_material(material_ptr);
                scene->add_obj(sphere_ptr);
                // build your sphere here from the parameters
                // i.e. you must call your sphere constructor and set its position
                // and radius from the above values. You must also put your new
                // sphere into the objects list (which can be global)
                // So something like;
                // mySphereClass *ms = new mySphereClass (x, y, z, r);   // make a new instance of your sphere class
                // ms->setMaterial (lastMaterialLoaded)
                // objectsList->push_back (ms);  // objectsList is a global std:vector<surface *> for example.
                
#ifdef IM_DEBUGGING
                // if we're debugging, show what we got:
                cout << "got a sphere with ";
                cout << "parameters: " << x << " " << y << " " << z << " " << r << endl;
#endif
                break;
                
            case 't':   // triangle
                break;
                
            case 'p':   // plane
                break;
                
                //
                // camera:
                //
            case 'c':   // camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
                float vx, vy, vz, d, iw, ih, pw, ph;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);
                vx = getTokenAsFloat (line, 4);
                vy = getTokenAsFloat (line, 5);
                vz = getTokenAsFloat (line, 6);
                d = getTokenAsFloat (line, 7);
                iw = getTokenAsFloat (line, 8);
                ih = getTokenAsFloat (line, 9);
                pw = getTokenAsFloat (line, 10);
                ph = getTokenAsFloat (line, 11);
                camera_ptr = new camera(x, y, z, vx, vy, vz, d, iw, ih, pw, ph);
                scene->set_camera(camera_ptr);
                //
                // lights:
                //
            case 'l':   // light
                
                // slightly different from the rest, we need to examine the second param,
                // which is at the third position on the line:
                switch (line[2]) {
                    case 'p':   // point light
                        float r, g, b;
                        x = getTokenAsFloat (line, 2);
                        y = getTokenAsFloat (line, 3);
                        z = getTokenAsFloat (line, 4);
                        r = getTokenAsFloat (line, 5);
                        g = getTokenAsFloat(line, 6);
                        b = getTokenAsFloat(line, 7);
#ifdef IM_DEBUGGING
                        std::cout << "plight x y z: " << x << " " << y << " " << z << std::endl;
#endif
                        plight_ptr = new plight(x, y, z, r, g, b);
                        scene->add_light(plight_ptr);
                        break;
                    case 'd':   // directional light
                        break;
                    case 'a':   // ambient light
                        break;
                        
                }
                
                break;
                
                //
                // materials:
                //
            case 'm':   // material
                // the trick here: we should keep a pointer to the last material we read in,
                // so we can apply it to any subsequent geometry. Say it's called "lastMaterialLoaded"
                // we migh then do something like this:
                //
                //  1. read in the 10 material parameters: dr, dg, db, sr, sg, sb, r, ir, ig, ib
                //  2. call lastMaterialLoaded->setMaterial(dr, dg, db,...);
                //
                float dr, dg, db, sr,sg,sb,ir,ig,ib;
                dr = getTokenAsFloat (line, 1);
                dg = getTokenAsFloat (line, 2);
                db = getTokenAsFloat (line, 3);
                sr = getTokenAsFloat (line, 4);
                sg = getTokenAsFloat (line, 5);
                sb = getTokenAsFloat (line, 6);
                r = getTokenAsFloat(line, 7);
                ir = getTokenAsFloat (line, 8);
                ig = getTokenAsFloat (line, 9);
                ib = getTokenAsFloat (line, 10);
                material_ptr.reset(new material(dr, dg, db, sr, sg, sb, r, ir, ig, ib));
#ifdef IM_DEBUGGING
                material_ptr->debug();
#endif
                break;
            case '/':
                // don't do anything, it's a comment
                break;
                
                //
                // options
                //
            case 'o':   // make your own options if you wish
                break;
        }
        
    }
}
