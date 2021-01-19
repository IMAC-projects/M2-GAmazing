#include "drawCircle.h"


int width{ 500 };
int height{ 500 };

void drawCircle(int x, int y, int r) {
    // Actual OpenGL calls
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    // Draw our line
    glBegin(GL_LINE_LOOP);
            glColor3f(0, 0, 1);

            // Framerate-based angel. As the frames are displayed, this integer will increment
            // and continue until it hits 360, then it will wrap around and start from 0
            static double iteration = 0;
            // The x, y offset onto the screen -- this should later be centered
            static const int offset = 150;
            // The radius of both our circle and the circle it's spinning in.

            // Calculate our x, y cooredinates
            double x1 = offset + r + x * cos(iteration);
            double y1 = offset + r + y * sin(iteration);
            static double wobble = 0.0;
            //double x2 = offset + 100 + radius * cos(iteration), y2 = offset + 100 * sin(iteration);
            y1 += sin(wobble) * 20;
            wobble += 0.01;
            
            static double b = 128;

            for (double i = 0; i < 2 * M_PI; i = i + ((2 * M_PI) / b))
            {
                #if 0
                    //   var = x pos + radius * cos(angle)
                    double x = x1 + radius * cos(i);
                    //   var = y pos + radius * sin(angle)
                    double y = y1 + radius * sin(i);
                    glVertex2f(x, y);
                #endif
                glVertex2f(x1 + r * cos(i), y1 + r * sin(i));
            }

            iteration += 0.01;

    glEnd();
}

void drawCa(int x, int y, int r)
{
    glfwInit();
 
    GLFWwindow * window = glfwCreateWindow(width, height, "Texture", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
    }
 
    glfwMakeContextCurrent(window);
 
    if (glewInit())
        std::cout << "Failed initializing GLEW\n";
 
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        drawCircle(x,y,r);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}