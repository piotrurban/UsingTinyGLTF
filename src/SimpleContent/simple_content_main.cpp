#include "SimpleContent.h"
#include <iostream>

int main()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 6);

	if (!glfwInit())
	{
		std::cerr << "glfwInit failed. Exiting\n";
		exit(-1);
	}
	GLFWwindow* window;
	int windowWidth = 600;
	int windowHeight = 600;
	window = glfwCreateWindow(windowWidth, windowHeight, "simple content", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit failed. Exiting\n";
		exit(-1);
	}

	SimpleContent sc;
	sc.loadShaders();

	glfwTerminate();
}