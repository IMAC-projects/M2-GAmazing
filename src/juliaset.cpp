#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>


#include <c3ga/Mvec.hpp>

#include "c3gaTools.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void computeFractal(const c3ga::Mvec<double> &translation, const c3ga::Mvec<double> &c, const c3ga::Mvec<float> zoom, int maxIter,
                    std::vector<unsigned char> &rgbBuffer, int width, int height) 
{
    int idx = 0;

    c3ga::Mvec<double> vec;
    vec[c3ga::E0] = c3ga::e12<double>();
    vec[c3ga::E1] = -0.835f;
    vec[c3ga::E2] = -0.2321f;

    // for each pixel in the image, evaluate fractal function:
    for (int imageY = 0; imageY < height; imageY++) {
        for (int imageX = 0; imageX < width; imageX++) {
            
            float imageXf = (float)(imageX-width/2);
            float imageYf = (float)(imageY-height/2);
            
            c3ga::Mvec<double> p;
            p[c3ga::E0] = c3ga::e12<double>();
            p[c3ga::E1] = imageXf;
            p[c3ga::E2] = imageYf;

            c3ga::Mvec<double> x = p;

            for (int i = 0; i < maxIter; i++) {
                x = x * x + c; // n = 2
                /*if (x.norm() > 1e4f) {
                    break;
                } // 1e4 = 'infinity'*/
            }
            
            // convert to grey-scale value:
            float valF = x.norm() / 10.0f;
            unsigned char val = (valF > 255) ? 255 : (unsigned char)(valF + 0.5f);

            rgbBuffer[idx + 0] = rgbBuffer[idx + 1] = rgbBuffer[idx + 2] = val;
            idx += 3;
        }
    }
    std::cout << rgbBuffer.data() << std::endl;
    stbi_write_png("export.png", 512, 512, 3, rgbBuffer.data(), 512 * 3);
}

int main()
{
    c3ga::Mvec<double> g_position = c3ga::point<double>(-2,0,0.5);
    c3ga::Mvec<double> g_c = c3ga::point<double>(-2,0,0.5);
    c3ga::Mvec<double> g_zoom =  0.07f;
    int g_maxIter = 30;

    int width = 512;
    int height = 512;
    std::vector<unsigned char>buf(width * height * 3);

    computeFractal(g_position, g_c, g_zoom, g_maxIter, buf, width, height);

    return 0;
}