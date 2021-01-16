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

unsigned char* circleSphere(std::vector<unsigned char> &rgbBuffer, int width, int height) 
{
    // a primal circle
    c3ga::Mvec<double> C = c3ga::point<double>(1,2,2.0) 
                              ^ c3ga::point<double>(2,0,2.0) 
                              ^ c3ga::point<double>(1,0,2.0);   


    for(int i = 0; i < 30; i++) 
    {
        c3ga::Mvec<double> line = c3ga::randomPoint<double>() 
                                ^ c3ga::randomPoint<double>()  
                                ^ c3ga::ei<double>();
        c3ga::Mvec<double> dualSphere = (!(line)) | C;
        c3ga::Mvec<double> ray = dualSphere | dualSphere;

        const bool intersect = (ray >= 0) ? false : true;

        for (int imageX = 0; imageX < width; imageX++) 
        {
            for (int imageY = 0; imageY < height; imageY++) {
                size_t index = imageX*width + imageY;
                if(intersect && int(dualSphere.norm()) <= 1) 
                {
                    rgbBuffer[index] = int(dualSphere.norm() * 20);
                    rgbBuffer[index+1] = int(dualSphere.norm() * 20);
                    rgbBuffer[index+2] = int(dualSphere.norm() * 20);

                } else {
                    rgbBuffer[index] = 50;
                    rgbBuffer[index+1] = 50;
                    rgbBuffer[index+2] = 50;

                }
                    
            }
        }
    }
    
    return rgbBuffer.data();

}

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
                if(z > 4.0) {
                    break;
                }
            }
            rgbBuffer[(imageY * width + imageX) * 3] = idx;
        }
    }
    return rgbBuffer.data();
}

unsigned char* tvThing(std::vector<unsigned char> &rgbBuffer, int max, int width, int height)
{
    int n = 50;
    for (int imageX = 0; imageX < width; imageX++) 
    {
        for (int imageY = 0; imageY < height; imageY++) {
            size_t index = imageX*width + imageY;
            auto z = c3ga::e01<double>();
            auto c = imageX * 1.75-1 + imageY * (c3ga::e1i<double>());
            while(z <= 1 && n <=0) {
                z = z * z  + c;
                std::cout << z << std::endl;
                n--;
            }
            if((int)z == 0) {
                rgbBuffer[index] = c;
            } else {
                rgbBuffer[index] = 50.0f;
            }
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
    //unsigned char* rgbBuffer = computeMandel(buf, g_maxIter, width, height);
    unsigned char* rgbBuffer = circleSphere(buf, width, height);
    displayTexture(rgbBuffer,width,height,3);
    return 0;
}