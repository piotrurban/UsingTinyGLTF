#include "GLFWUtils.h"

void resize_callback(GLFWwindow* window, int width, int height)
{
	GLFWwindow* save_context = glfwGetCurrentContext();
	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
	glfwMakeContextCurrent(save_context);
}

void registerResizeCallback(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, &resize_callback);
}
