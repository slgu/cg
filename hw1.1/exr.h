#pragma once
#include <iostream>
#include <ImfRgbaFile.h>
#include <ImathBox.h>
#include <ImfArray.h>
using namespace std;
using namespace Imf;
using namespace Imath;
using namespace Imf_2_2;

inline void write_exr_file(std::string filename, const Rgba * pixels, int width, int height) {
    RgbaOutputFile file (filename.c_str(), width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height);
}
