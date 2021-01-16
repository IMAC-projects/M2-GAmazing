#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <c3ga/Mvec.hpp>

#include "c3gaTools.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utils/stb_image_write.h"

#include "utils/displayTexture.h"

unsigned char* computeMandel(std::vector<unsigned char> &rgbBuffer, int max, int width, int height)
{
    for(int imageY = 0; imageY < height; imageY++) {
        for (int imageX = 0; imageX < width; imageX++) {

            float imageXf = (float)(imageX-width/2);
            float imageYf = (float)(imageY-height/2);
            c3ga::Mvec<double> c;
            c[c3ga::E0] = c3ga::e12<double>();
            c[c3ga::E1] = imageXf;
            c[c3ga::E2] = imageYf;
            auto z = c;
            int idx = 0;
            while(idx < max) {
                z = z + c;
                idx = idx + 1;
                std::cout << z << std::endl;
                if(z > 4.0) {
                    break;
                }
            }
            rgbBuffer[(imageY * width + imageX) * 3] = idx;
        }
    }
    return rgbBuffer.data();
}

unsigned char* computeRedFractal(const c3ga::Mvec<double> &translation, const c3ga::Mvec<double> &c, const c3ga::Mvec<float> zoom, int maxIter,
                    std::vector<unsigned char> &rgbBuffer, int width, int height) 
{
    int idx = 0;

    // for each pixel in the image, evaluate fractal function:
    for (int imageY = 0; imageY < height; imageY++) {
        for (int imageX = 0; imageX < width; imageX++) {
            
            float imageXf = (float)(imageX-width/2);
            float imageYf = (float)(imageY-height/2);
            
            c3ga::Mvec<double> p;
            p[c3ga::E0] = c3ga::e0<double>();
            p[c3ga::E1] = imageXf;
            p[c3ga::E2] = imageYf;

            c3ga::Mvec<double> x = p;

            for (int i = 0; i < maxIter; i++) {
                x = x * x + c; // n = 2
                if (x.norm()/10.0f > 20) {
                    break;
                }
            }
            
            float valF = x.norm() / 10.0f;
            //unsigned char val = (valF > 255) ? 255 : (unsigned char)(valF + 0.5f);

            rgbBuffer[idx] = 255.0f;
            rgbBuffer[idx + 1] = rgbBuffer[idx + 2] = valF;
            idx += 3;
        }
    }
    //stbi_write_png("export.png", 512, 512, 3, rgbBuffer.data(), 512 * 3);
    return rgbBuffer.data();
}


unsigned char* computeFractal(const c3ga::Mvec<double> &translation, const c3ga::Mvec<double> &c, const float zoom, int maxIter,
                    std::vector<unsigned char> &rgbBuffer, int width, int height) 
{
    int idx = 0;

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
                x = x * c3ga::e1<double>() * x + c;// n = 2
                if (x.norm() /100.0f > 1e04f) {
                    break;
                }
            }
            
            float valF = (x.norm() / 1e10f) * 100;
            unsigned char val = (valF > 255) ? 255 : (unsigned char)(valF + 0.5f);

            rgbBuffer[idx + 0] = rgbBuffer[idx + 1] = rgbBuffer[idx + 2] = val;
            idx += 3;
        }
    }
    //stbi_write_png("export.png", 512, 512, 3, rgbBuffer.data(), 512 * 3);
    return rgbBuffer.data();
}

int main()
{
    c3ga::Mvec<double> g_c = c3ga::point<double>(c3ga::e12<double>(),-0.9,-0.3);
    float g_zoom =  0.07f;
    int g_maxIter = 30;
    c3ga::Mvec<double> g_position = c3ga::point<double>(100.0f * g_zoom);

    int width = 1080;
    int height = 1080;
    std::vector<unsigned char>buf(width * height * 3);

    //unsigned char* rgbBuffer = computeFractal(g_position, g_c, g_zoom, g_maxIter, buf, width, height);
    unsigned char* rgbBuffer = computeMandel(buf, g_maxIter, width, height);
    displayTexture(rgbBuffer,width,height,3);
    return 0;
}