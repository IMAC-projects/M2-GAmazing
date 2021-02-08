#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 1080
 
class Shader
{
public:
    unsigned int program_ID;
    Shader(const char * vertex_shader_path, const char * fragment_shader_path);
    ~Shader();
 
    void use_shader();
 
    void set_float(const std::string & name, float value) const;
    void set_vec4(const std::string & name, glm::vec4 vec) const;
 
private:
    std::string read_shader_file(const char * file_path);
    void add_shader(unsigned int program, const char * shader_path, GLenum shader_type);
};