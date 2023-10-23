#pragma once
#include <gl_includes.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <array>

void reshapeFunc(GLFWwindow* window, int w, int h);

void triangleRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy);

void testTriangleRayCast();
