#include "displayTexture.h"

int screen_width{ 1080 };
int screen_height{ 1080 };
float center_x{ 0.0f };
float center_y{ 0.0f };
float zoom{ 1.0 };

void framebuffer_size_callback(GLFWwindow * window, int screen_width, int screen_height)
{
    glViewport(0, 0, screen_width, screen_height);
}

void process_input(GLFWwindow * window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
 
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        center_y = center_y + 0.01f * zoom;
        if (center_y > 1.0f)
            center_y = 1.0f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        center_y = center_y - 0.01f * zoom;
        if (center_y < -1.0f)
            center_y = -1.0f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        center_x = center_x - 0.01f * zoom;
        if (center_x < -1.0f)
            center_x = -1.0f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        center_x = center_x + 0.01f * zoom;
        if (center_x > 1.0f)
            center_x = 1.0f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        zoom = zoom * 1.04f;
        if (zoom > 1.0f)
            zoom = 1.0f;
    }
 
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        zoom = zoom * 0.96f;
        if (zoom < 0.00001f)
            zoom = 0.00001f;
    }
}

glm::vec4 find_ranges(std::vector<float> & data)
{
    std::sort(data.begin(), data.end());
    int lowest = 0;
    while (data[lowest] == 0.0f)
    {
        ++lowest;
    }
 
    int size = data.size();
    int length = size - lowest;
    glm::vec4 ranges = glm::vec4( data[lowest], data[lowest + length * 3 / 4 - 1], data[lowest + length * 7 / 8 - 1], data[size - 1] );
    return ranges;
}

void displayTexture(unsigned char* loadedTexture, int width, int height, int channels)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow * window = glfwCreateWindow(screen_width, screen_height, "Texture", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
    }
 
    glfwMakeContextCurrent(window);
 
    if (glewInit())
        std::cout << "Failed initializing GLEW\n";
 
    glViewport(0, 0, screen_width, screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    Shader our_shader("./src/shader/displayTexture.vert", "./src/shader/displayTexture.frag");
    
    // data for a fullscreen quad
    GLfloat vertices[] = {
    //  X     Y     Z           U     V     
       1.0f, 1.0f, 0.0f,       1.0f, 1.0f, // vertex 0
      -1.0f, 1.0f, 0.0f,       0.0f, 1.0f, // vertex 1
       1.0f,-1.0f, 0.0f,       1.0f, 0.0f, // vertex 2
      -1.0f,-1.0f, 0.0f,       0.0f, 0.0f, // vertex 3
    }; // 4 vertices with 5 components (floats) each
 
    GLint texture_location = glGetUniformLocation(our_shader.program_ID, "tex");
    unsigned int IBO, VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
 
    glBindVertexArray(VAO);
 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4*5, vertices, GL_STATIC_DRAW); 

    // set up generic attrib pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (char*)0 + 0*sizeof(GLfloat));
 
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (char*)0 + 3*sizeof(GLfloat));    

    
    // generate and bind the index buffer object
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            
    GLuint indexData[] = {
        0,1,2, // first triangle
        2,1,3, // second triangle
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*2*3, indexData, GL_STATIC_DRAW);
 
    //unbind vao
    glBindVertexArray(0);

    // texture handle
    GLuint texture;
    
    // generate texture
    glGenTextures(1, &texture);

    // bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, &loadedTexture[0]);
 
    glEnable(GL_DEPTH_TEST);

    std::vector<float> pixel_data(screen_width * screen_height, 0.0f);
    glm::vec4 ranges = glm::vec4(0.0001f, 0.33333f, 0.66667f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        process_input(window);
 
        our_shader.use_shader();
        // bind texture to texture unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        // set texture uniform
        glUniform1i(texture_location, 0);
        
        // bind the vao
        glBindVertexArray(VAO);
        
        // draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
        ranges = find_ranges(pixel_data);
    }
    
    glDeleteTextures(1, &texture);
 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
 
    glfwTerminate();
}