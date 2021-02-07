#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>

#include <c3ga/Mvec.hpp>

#include "utils/c3gaTools.hpp"
#include "utils/circle.h"
#include "utils/displayTexture.h"
#include "utils/flower.h"
#include "utils/sierpinski.h"

void circle() 
{
    // a primal circle
    c3ga::Mvec<double> C = c3ga::point<double>(1,2,2.0) 
                              ^ c3ga::point<double>(2,0,2.0) 
                              ^ c3ga::point<double>(1,0,2.0);   
    double radius;
    c3ga::Mvec<double> center, direction;
    extractDualCircle(C.dual(), radius,  center, direction);
    auto equation = "Circle( Point({" + std::to_string(center[c3ga::E1]) + ","  + std::to_string(center[c3ga::E2]) + ","  + std::to_string(center[c3ga::E3]) + "}), "
                + std::to_string(fabs(radius))
                + ", Vector((" + std::to_string(direction[c3ga::E1]) + ","  + std::to_string(direction[c3ga::E2]) + ","  + std::to_string(direction[c3ga::E3]) + ")))";

    std::cout << equation << std::endl;
    displayCircle(center[c3ga::E1],center[c3ga::E2],radius*100);
}

void flower() 
{
    // sphere
    c3ga::Mvec<double> sphere = c3ga::point<double>(0,0,2.0) 
                              ^ c3ga::point<double>(2,0,2.0) 
                              ^ c3ga::point<double>(1,1,2.0) 
                              ^ c3ga::point<double>(1,0,3.0);
    // dualize
    sphere = sphere.dual();

    // extract parameters
    sphere /= sphere[c3ga::E0]; // back to correct scale
    double squareRadius = sphere | sphere;
    double radius = sqrt(squareRadius);
    c3ga::Mvec<double> center = sphere;
    center /= center[c3ga::E0];
    c3ga::Mvec<double> direction;
    direction[c3ga::E0] =  center[c3ga::E0];
    displayFlower(radius,direction[c3ga::E0]);
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
            p[c3ga::E0] = c3ga::e2<double>();
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
            float val = (valF > 255) ? 255 : (valF);
            rgbBuffer[idx + 0] = 0.0f;
            rgbBuffer[idx + 1] = int(val/5);
            rgbBuffer[idx + 2] = val;
            idx += 3;
        }
    }
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
    unsigned char* rgbBuffer = computeRedFractal(g_position, g_c, g_zoom, g_maxIter, buf, width, height);
    circle();
    //flower();
    //displayTexture(rgbBuffer,width,height,3);
    //sierpinski();
    return 0;
}