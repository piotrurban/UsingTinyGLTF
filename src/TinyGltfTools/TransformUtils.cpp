#include "TransformUtils.h"
#include "gl_includes.h"


#include <vector>
#include <array>
#include <iostream>
#include <sstream>
#include <format>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

void reshapeFunc(GLFWwindow* window, int w, int h) {
	(void)window;
	int fb_w, fb_h;
	glfwGetFramebufferSize(window, &fb_w, &fb_h);
	glViewport(0, 0, fb_w, fb_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void triangleRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy)
{
	const glm::vec3 normal = glm::cross(triangle_verts.at(1) - triangle_verts.at(0), triangle_verts.at(2) - triangle_verts.at(0));
	const float sum0 = glm::dot(normal, triangle_verts.at(0));
	const float sum1 = glm::dot(normal, triangle_verts.at(1));
	const float sum2 = glm::dot(normal, triangle_verts.at(2));
	std::cout << std::format("sum0,1,2: {}, {}, {}", sum0, sum1, sum2) << std::endl;

	const float cz = sum0 - glm::dot(normal, glm::vec3(projection_xy, 0));
	if (normal.z != 0)
	{
		const float z = cz / normal.z;
		std::cout << std::format("intersection:  z = {0}, (cz = {1})\n", z, cz);
		// dot products of P - A, P - B and P - C, P = (x,y,z)
		const glm::vec3 p(projection_xy, z);
		if ((glm::dot(triangle_verts.at(0) - p, triangle_verts.at(1) - p) < 0) ||
			(glm::dot(triangle_verts.at(0) - p, triangle_verts.at(2) - p) < 0) ||
			(glm::dot(triangle_verts.at(1) - p, triangle_verts.at(2) - p) < 0)
			)
		{
			std::cout << "Ray cast hit\n";
		}
		else
		{

			std::cout << "Ray cast missed\n";
			std::cout << std::format("Dot products:\n {}, {}, {}\n",
				glm::dot(triangle_verts.at(0) - p, triangle_verts.at(1) - p),
				glm::dot(triangle_verts.at(0) - p, triangle_verts.at(2) - p),
				glm::dot(triangle_verts.at(1) - p, triangle_verts.at(2) - p));

		}
	}
}

std::string to_string(const glm::vec3& vec3)
{
	std::stringstream oss;
	oss << std::format("( {}, {}, {})", vec3.x, vec3.y, vec3.z);
	return oss.str();
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec3)
{
	os << to_string(vec3);
	return os;
}
glm::vec3 triangleZRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy)
{
	const glm::vec3 normal = glm::cross(triangle_verts.at(1) - triangle_verts.at(0), triangle_verts.at(2) - triangle_verts.at(0));
	// Die Ebene des Dreiecks: a*x + b*y + c*z + d = 0
	// normal = [a, b, c]
	const float d = glm::dot(normal, triangle_verts.at(0));
	// finde den Schnittpunkt (x,y,z)
	const float cz = d - glm::dot(normal, glm::vec3(projection_xy, 0));
	if (normal.z != 0)
	{
		const float z = cz / normal.z;
		const glm::vec3 intersection_point(projection_xy, z);
		const auto pa = triangle_verts.at(0) - intersection_point;
		const auto pb = triangle_verts.at(1) - intersection_point;
		const auto pc = triangle_verts.at(2) - intersection_point;

		// for at least 2 vertices, 
		if (glm::dot(glm::cross(pa,pb), glm::cross(pc,pb)) <0 && dot(glm::cross(pa,pb), glm::cross(pa,pc)) < 0)
		{
			return intersection_point;
		}
		else
		{
			return glm::vec3{ 0.0, 0.0, std::numeric_limits<float>::max() };
		}
	}
	else
	{
		return glm::vec3{ 0.0, 0.0, std::numeric_limits<float>::max() };
	}
}

void testTriangleRayCast()
{
	const glm::vec3 a{ 0.0, 10.0, -1212.0 };
	const glm::vec3 b{ 10.0, 0.0, 10.0 };
	const glm::vec3 c{ 0.0, 0.0, 101.0 };
	const auto triangle = std::array<glm::vec3, 3>{a, b, c};
	for (float x = 0.0; x < 20.0; x += 1.0)
	{
		const glm::vec2 projection_xy{ x, 9.0 - x };
		triangleRayCast(triangle, projection_xy);

	}
}

std::pair<float, float> windowToNormCoord(int win_x, int win_y, GLFWwindow* window)
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return std::make_pair(2.0F * static_cast<float>(win_x) / static_cast<float>(width) - 1.0F, 1.0F - 2.0F * static_cast<float>(win_y) / static_cast<float>(height));
}

void onMouseClickCbk(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const auto [x, y] = windowToNormCoord(xpos, ypos, window);
		std::cout << std::format("Mouse clicked at x = {}, y = {}\n", x, y);
	}
}

bool isTriangleInViewport(const std::array<glm::vec3, 3>& triangle)
{
	for (const glm::vec3& vertex : triangle)
	{
		if (std::abs(vertex.x) <= 1.0F && std::abs(vertex.y) <= 1.0F)
		{
			return true;
		}
	}
	return false;
}