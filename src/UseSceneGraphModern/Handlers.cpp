#include "Handlers.h"
#include <TransformUtils.h>
#include <ContentUtils.h>

#include "NodeVisitors.h"

#include <iostream>
#include <format>

const Content* g_currentContent;

void setCurrentContent(const Content& content)
{
	g_currentContent = &content;
}

void onMouseClickRayCast(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const auto [x, y] = windowToNormCoord(xpos, ypos, window);
		const float zToMesh0 = nearestZRayCast(x, y, *g_currentContent, 0);
		std::cout << std::format("RayCast: mouse clicked at x = {}, y = {}, z to mesh 0 = {}\n", x, y, zToMesh0);
	}
}