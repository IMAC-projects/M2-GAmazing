#include <vector>
#include <algorithm>
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow * window, int screen_width, int screen_height);
void process_input(GLFWwindow * window);
glm::vec4 find_ranges(std::vector<float> & data);
void displayTexture(unsigned char* loadedTexture, int width, int height, int channels);