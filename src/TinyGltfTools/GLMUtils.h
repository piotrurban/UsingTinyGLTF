#pragma once
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

inline glm::vec4 perspectiveDivide(const glm::vec4& vec4);
inline glm::vec3 perspectiveDivide3(const glm::vec4& vec4);
glm::vec4 perspectiveDivide(const glm::vec4& vec4)
{
	return vec4 / vec4.w;
}

glm::vec3 perspectiveDivide3(const glm::vec4& vec4)
{
	return glm::vec3(perspectiveDivide(vec4));
}