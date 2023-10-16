#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include <GL/glew.h>

#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <tiny_gltf.h>

GLFWwindow* window;

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	
	if (!glfwInit())
	{
		printf("glfwInit failed. Exiting\n");
		exit(1);
	}
	window = glfwCreateWindow(500, 300, "Future cube window", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	
}