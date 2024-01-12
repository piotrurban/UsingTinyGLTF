#pragma once
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <filesystem>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <any>

class SimpleContent
{
public:
	SimpleContent(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices,
		GLenum mode = GL_TRIANGLES,
		const std::optional<std::filesystem::path> vertexShaderPath = {},
		const std::optional<std::filesystem::path> fragmentShaderPath = {},
		const std::optional<std::filesystem::path> texturePath = {},
		const std::unordered_set<std::string>& uniforms = {},
		const std::unordered_map<std::string, std::any>& anyUniforms = {});
	virtual ~SimpleContent();

	void setMV(const glm::mat4& mv);
	void setUniform(const std::string& name, const std::any value);
	void draw();
	void cleanup();

	const glm::mat4& getMV() { return m_mv; }
	static const std::filesystem::path s_defaultVertexShader;
	static const std::filesystem::path s_defaultFragmentShader;
	static const std::filesystem::path s_circleFragmentShader;
	static const std::filesystem::path s_distanceFragmentShader;
private:
	void setupVertices();
	void setupOffscreenBuffers();
	void loadShaders();
	std::vector<std::pair<std::string, GLenum>>  getShaderUniforms();
	void setAnyUniformsFromShaderCode();

	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned short> m_indices;
	GLenum m_mode;
	std::optional<std::filesystem::path> m_vertexShaderPath;
	std::optional<std::filesystem::path> m_fragmentShaderPath;
	std::optional<std::filesystem::path> m_texturePath;
	glm::mat4 m_mv;
	GLuint m_vertexBuffer{  };
	GLuint m_indicesBuffer;
	GLuint m_vertexArray;
	GLuint m_indexArray;
	GLuint m_tex;
	GLuint m_prog;
public:
	GLuint m_fbo1;
	GLuint m_fbo2;
	GLuint m_tex1;
	GLuint m_tex2;

public:
	std::unordered_map<std::string, float> m_uniformMap;
	std::unordered_map<std::string, std::any> m_anyUniformMap;
	std::unordered_map<std::string, GLuint> m_uniformLoc;
	std::vector<std::pair<std::string, GLenum>> m_uniforms;
	GLint m_uniformTextureSamplerLoc;
};