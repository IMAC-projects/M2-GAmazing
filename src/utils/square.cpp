#include "square.h"
#include "gl-exception.h"

std::array<glm::vec3,6> squareVertices()
{    
    c3ga::Mvec<float> a = c3ga::point<float>(-0.5f, -0.5f, 0);
    c3ga::Mvec<float> b = c3ga::point<float>(0.5f, -0.5f, 0);
    c3ga::Mvec<float> c = c3ga::point<float>(0.5f, 0.5f, 0);
    c3ga::Mvec<float> d = c3ga::point<float>(-0.5f, 0.5f, 0);
    c3ga::Mvec<float> plane = a ^ b ^ c ^ c3ga::ei<float>();
    c3ga::Mvec<float> normal = plane * a;

    c3ga::Mvec<float> orientation = (c3ga::ei<float>() | (!normal)) ^ c3ga::ei<float>();
    orientation = orientation | c3ga::e0<float>();
    orientation /= orientation.norm();

    // Retriving vertices position in euclidean space
    c3ga::Mvec<float> a0 = a / a[c3ga::E0];
    c3ga::Mvec<float> b0 = b / b[c3ga::E0];
    c3ga::Mvec<float> c0 = c / c[c3ga::E0];
    c3ga::Mvec<float> d0 = d / d[c3ga::E0];
    glm::vec3 aPos = { a0[c3ga::E1], a0[c3ga::E2], a0[c3ga::E3] };
    glm::vec3 bPos = { b0[c3ga::E1], b0[c3ga::E2], b0[c3ga::E3] };
    glm::vec3 cPos = { c0[c3ga::E1], c0[c3ga::E2], c0[c3ga::E3] };
    glm::vec3 dPos = { d0[c3ga::E1], d0[c3ga::E2], d0[c3ga::E3] };
          
    std::array<glm::vec3,6> vertices = {
            (aPos),
            (bPos),
            (cPos),
            (cPos),
            (dPos),
            (aPos),
    };
    return vertices;
}


void displaySquare()
{
    glfwInit();
    int width = SCREEN_WIDTH;
    int height = SCREEN_HEIGHT;
 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Square", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
    }
 
    glfwMakeContextCurrent(window);
 
    if(glewInit())
    {
        std::cout << "Failed initializing GLEW\n";
    }

	glEnable(GL_DEPTH_TEST);
	Shader shader("../src/shader/flower.vert", "../src/shader/color.frag");

    auto vertices = squareVertices();

    unsigned int VAO, VBO;
    GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glBindVertexArray(VAO));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3),vertices.data(), GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),(void*)0));
    GLCall(glEnableVertexAttribArray(0));
 
    while (!glfwWindowShouldClose(window))
    {

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(window, &width, &height);
        shader.use_shader();
        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(6)));
        GLCall(glBindVertexArray(0));
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
}
