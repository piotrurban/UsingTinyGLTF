#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class SimpleContent
{
public:
	SimpleContent() = default;
	void setupVertices(const std::vector<glm::vec3>& vertices);
	void loadShaders();
	void setMV(const glm::mat4& mv);
	void draw();

	std::vector<glm::vec3> m_vertices;
	glm::mat4 m_mv;
	GLuint m_vertexBuffer;
	GLuint m_indicesBuffer;
};