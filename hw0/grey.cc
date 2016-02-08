/*************************************************************************
    > File Name: grey.cpp
    > Author: Gu Shenlong
    > Mail: blackhero98@gmail.com
    > Created Time: Sun Jan 31 21:04:14 2016
 ************************************************************************/

#include "grey.h"
void read_exr_file(std::string filename, Array2D <Rgba>  & pixels, int & width, int & height) {
    RgbaInputFile file(filename.c_str());
    Box2i dw = file.dataWindow();
    width = dw.max.x - dw.min.x + 1;
    height = dw.max.y - dw.min.y + 1;
    pixels.resizeErase(height, width);
    file.setFrameBuffer(&pixels[0][0] -dw.min.x - dw.min.y * width, 1, width);
    file.readPixels(dw.min.y, dw.max.y);
}

void write_exr_file(std::string filename, const Rgba * pixels, int width, int height) {
    RgbaOutputFile file (filename.c_str(), width, height, WRITE_RGBA);
    file.setFrameBuffer (pixels, 1, width);
    file.writePixels (height); 
}

void grey_change(Array2D <Rgba> & pixels, int width, int height) {
    for (int i = 0; i < height / 3; ++i)
        for (int j = 0; j < width / 3; ++j) {
            Rgba & rgb = pixels[height / 3 + i][width / 3 + j]; 
            double l = rgb.r * 0.2126 + 0.7152 * rgb.g + 0.0722 * rgb.b;
            rgb.r = rgb.g = rgb.b = l;
        }
}

int main(int argc, char *argv[]){
    Array2D <Rgba> pixels;
    int width;
    int height;
    if (argc != 2) {
        puts("give a image file name as param");
        return 1;
    }
    string filename(argv[1]);
    int len = filename.size();
    if (len <= 4 || filename.substr(len - 4, 4) != ".exr") {
        puts("file format not exr");
        return 1;
    }
    read_exr_file(filename, pixels, width, height);
    grey_change(pixels, width, height);
    write_exr_file(filename.substr(0, len - 4) + "_out.exr", &pixels[0][0], width, height);
    return 0;
}
