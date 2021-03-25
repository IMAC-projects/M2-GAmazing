#include "circle.h"
#include "shader.h"

void drawCircle(int x, int y, int r) {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    // Draw our line
    glBegin(GL_LINE_LOOP);
            glColor3f(1, 0, 1);

            // framerate-based 
            static double iteration = 0;
            // The x, y offset onto the screen
            static const int offset = 500;
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

void displayCircle(int x, int y, int r)
{
    glfwInit();
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
 
    GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Circle", NULL, NULL);
 
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
        drawCircle(x*2,y*2,r*2);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}