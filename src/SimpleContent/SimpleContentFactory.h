#pragma once
#include "SimpleContent.h"
#include <filesystem>
namespace fs = std::filesystem;

struct SimpleContentConfig
{
	fs::path m_vertexShaderPath{ SimpleContent::s_defaultVertexShader };
	fs::path m_fragmentShaderPath{ SimpleContent::s_defaultFragmentShader };
	fs::path m_texturePath{ "" };
	std::vector<glm::vec3> m_vertices{};
	std::vector<unsigned short> m_indices{};
	GLenum m_mode{GL_TRIANGLES};
	std::unordered_set<std::string> m_uniforms{};
	std::unordered_map<std::string, float> m_uniformDefaults{};
	std::unordered_map<std::string, std::any> m_anyUniforms{};
	glm::mat4 m_MVDefault{ glm::mat4(1.0) };
};

enum class SimpleContentType
{
	DEFAULT,
	CIRCLE,
	SQUARE,
};

SimpleContentConfig getSimpleContentConfig(SimpleContentType type);

SimpleContent getSimpleContent(SimpleContentType type);

class SimpleContentBuilder
{
public:
	SimpleContentBuilder() = default;

	void setConfig(const SimpleContentConfig& config);
	void setType(const SimpleContentType type);
	void setVertexShaderPath(const fs::path& vertexShaderPath);
	void setFragmentShaderPath(const fs::path& fragmentShaderPath);
	void setTexturePath(const fs::path& texturePath);
	void setVertices(const std::vector<glm::vec3>& vertices);
	void setIndices(std::vector<unsigned short>& indices);
	void setUniforms(const std::unordered_set<std::string>& uniforms);
	template<class T>
	void addAnyUniform(const std::string& name);

	SimpleContent build();

	SimpleContentConfig m_config{};

};

template<class T>
void SimpleContentBuilder::addAnyUniform(const std::string& name)
{
	m_config.m_anyUniforms[name] = T{};
}