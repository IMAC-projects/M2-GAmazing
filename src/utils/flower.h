#define PI 3.1415926

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
using namespace std;

std::vector<float> drawglobeVBO(int radius, int direction);
std::vector<int> drawglobeEBO(int radius, int direction);
void drawFlower(int radius, int direction);