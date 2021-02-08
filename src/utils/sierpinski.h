#include "shader.h"

const int numPoints = 1000000;

float ZOOM = {1.0f};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{        
        ZOOM = ZOOM * 1.04f;
        if (ZOOM > 50.0f)
            ZOOM = 1.0f;
	}

	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{        
        ZOOM = ZOOM * 0.96f;
        if (ZOOM < 0.00001f)
            ZOOM = 0.00001f;
	}
}

void sierpinski()
{
    // Initialise GLFW
    glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Sierpinski", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	
	glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    if(glewInit()) {
		std::cout << "Failed initializing GLEW\n";
	}

    // Load shaders and use the resulting shader program
	Shader shader("../src/shader/zoom.vert", "../src/shader/color.frag");

    glm::vec2 points[numPoints];

    // Specifiy the vertices for a triangle
    glm::vec2 vertices[3] = {
        glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0), glm::vec2(1.0, -1.0)
    };

    // Select an arbitrary initial point inside of the triangle
    points[0] = glm::vec2( 2.5, 3);

    // compute and store N-1 new points
    for ( int i = 1; i < numPoints; ++i ) {
        int j = rand() % 3;   // pick a vertex at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = ( points[i - 1]/2.0f + vertices[j]/2.0f);
    }

    // Create a vertex array object
    GLuint vao[1];

    // Create and initialize a buffer object
    GLuint buffer;
    glGenVertexArrays( 1, vao );
    glGenBuffers( 1, &buffer );
    glBindVertexArray( vao[0] );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, numPoints * sizeof(float), points, GL_STATIC_DRAW );


    // Initialize the vertex position attribute from the vertex shader
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),(GLvoid*) (0));
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)){
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear( GL_COLOR_BUFFER_BIT );
        shader.use_shader();
        shader.set_float("zoom",ZOOM);
        glBindVertexArray(vao[0]);
        glDrawArrays(GL_POINTS, 0, numPoints);    // draw the points
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}