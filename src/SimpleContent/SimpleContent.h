#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <filesystem>
#include <optional>

class SimpleContent
{
public:
	SimpleContent(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices,
		GLenum mode = GL_TRIANGLES,
		const std::optional<std::filesystem::path> vertexShaderPath = {},
		const std::optional<std::filesystem::path> fragmentShaderPath = {});
	void setMV(const glm::mat4& mv);
	void draw();

	static const std::filesystem::path s_defaultVertexShader;
	static const std::filesystem::path s_defaultFragmentShader;
	static const std::filesystem::path s_circleFragmentShader;
private:
	void setupVertices();
	void loadShaders();

	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned short> m_indices;
	GLenum m_mode;
	std::optional<std::filesystem::path> m_vertexShaderPath;
	std::optional<std::filesystem::path> m_fragmentShaderPath;
	glm::mat4 m_mv;
	GLuint m_vertexBuffer{  };
	GLuint m_indicesBuffer;
	GLuint m_prog;
};