#include "flower.h"
#include "shader.h"
#include <time.h>

std::vector<float> drawglobeVBO(int radius, int direction){
	vector<float> c;

	int slices = 15;
	srand (time(NULL));

	//when rand() % 2 + 1 is equal to 1 it's super fun !
	float angle = direction*rand() % 2 + 1*PI/slices;
	float numAngle = 0;
	for (int i = 0; i <= slices; i++)
	{
		for (float j = 0.0f; j <= radius; j++)
		{
			if ((radius-j)>=0)
			{
				float x = (radius-j) * glm::sin(numAngle);
				float y = (radius-j) * glm::cos(numAngle);
				numAngle += angle;
				c.push_back(x);
				c.push_back(y);
				c.push_back(0.0f);
				c.push_back(0.0f);
				c.push_back(0.0f);
				c.push_back(0.0f);
			}
			j += 0.25f;
		}

	}
	return c;
}

std::vector<int> drawglobeEBO(int radius, int direction){
	vector<int> ebo;
	vector<float> lc =drawglobeVBO(radius,direction);
	int length = lc.size()/3;
	for (int i=0;i<=length;i++)
	{
			if ((!(i%2))&&(i!=0))
			{
				ebo.push_back(i);
				ebo.push_back(i);
			} 
			else
			{
				ebo.push_back(i);
			}
		}
	return ebo;
}

void displayFlower(int radius, int direction) {
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flower", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	
	glfwMakeContextCurrent(window);

    if(glewInit()) {
		std::cout << "Failed initializing GLEW\n";
	}

	glEnable(GL_DEPTH_TEST);
	Shader shader("../src/shader/pos.vert", "../src/shader/color.frag");
	vector<float> mmc = drawglobeVBO(radius, direction);
	vector<int> mfc = drawglobeEBO(radius, direction);

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mmc.size()*sizeof(float),&mmc[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mfc.size()*sizeof(float),&mfc[0], GL_STATIC_DRAW);
	// &Vertor [0] refers to the address of this vector. &Vector does not refer to a vector address.

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window)){
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		shader.use_shader();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, mfc.size(), GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents(); 
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
	glfwTerminate();
}