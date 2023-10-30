#include "Handlers.h"
#include <TransformUtils.h>
#include <ContentUtils.h>

#include "NodeVisitors.h"
#include <SimpleContent/SimpleContent.h>

#include <iostream>
#include <format>

const Content* g_currentContent;
 SimpleContent* g_currentMousePointerIcon;

void setCurrentContent(const Content& content)
{
	g_currentContent = &content;
}

void setCurrentMousePointerIcon(SimpleContent& content)
{
	g_currentMousePointerIcon = &content;
}

void onMouseClickRayCast(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const auto [x, y] = windowToNormCoord(xpos, ypos, window);
		const float zToMesh0 = ZRayCast(x, y, *g_currentContent, 0);
		std::cout << std::format("RayCast: mouse clicked at x = {}, y = {}, z to mesh 0 = {}\n", x, y, zToMesh0);
		onMouseClickMoveCircle(window, button, action, mods);
	}
}

void onMouseClickMoveCircle(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const auto [x, y] = windowToNormCoord(xpos, ypos, window);
		if (g_currentMousePointerIcon != nullptr)
		{
			g_currentMousePointerIcon->setMV(glm::translate(glm::mat4(1.0F), glm::vec3(x, y, 0.0F)));
		}
	}
}