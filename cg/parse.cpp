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
#include "scene.h"
#include "surface.h"
#include "material.hpp"
#include "parse.h"
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
void Parser::parse_scene_file(char *filnam, shared_ptr <Scene> & scene)
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
        alight * alight_ptr;
        plane * plane_ptr;
        tri * tri_ptr;
        slight * slight_ptr;
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
                float x1,y1,z1,x2,y2,z2,x3,y3,z3;
                x1 = getTokenAsFloat(line, 1);
                y1 = getTokenAsFloat (line, 2);
                z1 = getTokenAsFloat (line, 3);
                x2 = getTokenAsFloat (line, 4);
                y2 = getTokenAsFloat (line, 5);
                z2 = getTokenAsFloat (line, 6);
                x3 = getTokenAsFloat (line, 7);
                y3 = getTokenAsFloat (line, 8);
                z3 = getTokenAsFloat (line, 9);
                tri_ptr = new tri(x1,y1,z1,x2,y2,z2,x3,y3,z3);
                tri_ptr->set_material(material_ptr);
                scene->add_obj(tri_ptr);
                break;
            case 'p':   // plane
                float d;
                x = getTokenAsFloat (line, 1);
                y = getTokenAsFloat (line, 2);
                z = getTokenAsFloat (line, 3);
                d = getTokenAsFloat (line, 4);
                plane_ptr = new plane(x, y, z, d);
                plane_ptr->set_material(material_ptr);
                scene->add_plane(plane_ptr);
                break;
                //
                // camera:
                //
            case 'c':   // camera
                // one trick here: the cameras pixel count (width, height) are integers,
                // so cast them.
                float vx, vy, vz, iw, ih, pw, ph;
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
                        
                        r = getTokenAsFloat (line, 2);
                        g = getTokenAsFloat(line, 3);
                        b = getTokenAsFloat(line, 4);
                        alight_ptr = new alight(r, g, b);
                        scene->add_light(alight_ptr);
                        break;
                    case 's':
                        float nx, ny, nz, ux, uy, uz, len;
                        //l s x y z nx ny nz ux uy uz len r g b
                        x = getTokenAsFloat (line, 2);
                        y = getTokenAsFloat (line, 3);
                        z = getTokenAsFloat (line, 4);
                        nx = getTokenAsFloat (line, 5);
                        ny = getTokenAsFloat (line, 6);
                        nz = getTokenAsFloat (line, 7);
                        ux = getTokenAsFloat (line, 8);
                        uy = getTokenAsFloat (line, 9);
                        uz = getTokenAsFloat (line, 10);
                        len = getTokenAsFloat (line, 11);
                        r = getTokenAsFloat (line, 12);
                        g = getTokenAsFloat (line, 13);
                        b = getTokenAsFloat (line, 14);
                        slight_ptr = new slight(x, y, z, nx, ny, nz, ux, uy, uz, len, r, g, b);
                        scene->add_light(slight_ptr);
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
            case 'o':   // make your own opti		ons if you wish
                break;
            case 'w': //load an obj file
                std::vector <int> tris;
                std::vector <float> verts;
                int idx = 2;
                while (line[idx] == ' ') ++idx;
                parse_obj_file(line.substr(idx).c_str(), tris, verts);
                int n = int(tris.size()) / 3;
                for (int i = 0; i < n; ++i) {
                    int idx1 = tris[3 * i];
                    int idx2 = tris[3 * i + 1];
                    int idx3 = tris[3 * i + 2];
                    tri_ptr = new tri(verts[3 * idx1],verts[3 * idx1 + 1], verts[3 * idx1 + 2],
                                      verts[3 * idx2],verts[3 * idx2 + 1], verts[3 * idx2 + 2],
                                      verts[3 * idx3],verts[3 * idx3 + 1], verts[3 * idx3 + 2]);
                    tri_ptr->set_material(material_ptr);
                    scene->add_obj(tri_ptr);
                }
                break;
        }
    }
}

void Parser::parse_obj_file (const char * fillnam, std::vector< int > &tris, std::vector< float > &verts) {
    // clear out the tris and verts vectors:
    tris.clear ();
    verts.clear ();
    ifstream in(fillnam);
    char buffer[1025];
    string cmd;
    for (int line=1; in.good(); line++) {
        in.getline(buffer,1024);
        buffer[in.gcount()]=0;
        cmd="";
        istringstream iss (buffer);
        
        iss >> cmd;
        
        if (cmd[0]=='#' or cmd.empty()) {
            // ignore comments or blank lines
            continue;
        }
        else if (cmd=="v") {
            // got a vertex:
            
            // read in the parameters:
            double pa, pb, pc;
            iss >> pa >> pb >> pc;
            
            verts.push_back (pa);
            verts.push_back (pb);
            verts.push_back (pc);
        }
        else if (cmd=="f") {
            // got a face (triangle)
            
            // read in the parameters:
            int i, j, k;
            iss >> i >> j >> k;
            
            // vertex numbers in OBJ files start with 1, but in C++ array
            // indices start with 0, so we're shifting everything down by
            // 1
            tris.push_back (i-1);
            tris.push_back (j-1);
            tris.push_back (k-1);
        }
        else {
            std::cerr << "Parser error: invalid command at line " << line << std::endl;
        }
        
    }
    in.close();
}
