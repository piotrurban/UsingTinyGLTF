#pragma once
#include <gl_includes.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>
#include <iostream>

void reshapeFunc(GLFWwindow* window, int w, int h);

void triangleRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy);

void testTriangleRayCast();

std::pair<float, float> windowToNormCoord(int win_x, int win_y, GLFWwindow* window);

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec3);

void onMouseClickCbk(GLFWwindow* window, int button, int action, int mods);
