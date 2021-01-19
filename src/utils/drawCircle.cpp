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
            glColor3f(1, 0, 1);

            // framerate-based 
            static double iteration = 0;
            // The x, y offset onto the screen
            static const int offset = 150;
            // The radius of both our circle and the circle it's spinning in.

            // Calculate our x, y cooredinates
            double x1 = offset + r + x * cos(iteration);
            double y1 = offset + r + y * sin(iteration);
            static double wobble = 0.0;
            x1 += cos(wobble) * 20;
            y1 += sin(wobble) * 50;
            wobble += 0.1;
            
            static double b = 128;

            for (double i = 0; i < 10 * M_PI; i = i + ((10 * M_PI) / b))
            {
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