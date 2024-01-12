#include "SimpleContentFactory.h"
#include <filesystem>
#include <generated_model/models.h>

namespace fs = std::filesystem;
using namespace std::string_literals;


SimpleContentConfig getSimpleContentConfig(SimpleContentType type)
{
	SimpleContentConfig config{};
	config.m_mode = GL_TRIANGLES;

	switch (type)
	{
	case SimpleContentType::DEFAULT:
	{
		config.m_vertices = { {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F }, {-1.0F, 1.0F, 0.0F} };
		config.m_indices = { 0, 1, 3, 3, 1, 2 };
		break;
	}
	case SimpleContentType::CIRCLE:
	{
		config.m_vertices = { {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F }, {-1.0F, 1.0F, 0.0F} };
		config.m_indices = { 0, 1, 3, 3, 1, 2 };
		config.m_uniforms = { "u_radius"s };
		config.m_uniformDefaults = { {"u_radius", 0.25F} };
		config.m_MVDefault = glm::mat4(1.0F);
		break;
	}
	case SimpleContentType::SQUARE:
	{
		config.m_vertices = { {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F }, {-1.0F, 1.0F, 0.0F} };
		config.m_indices = { 0, 1, 3, 3, 1, 2 };
		config.m_MVDefault = glm::mat4(1.0F);
		break;
	}
	}

	return config;
}

SimpleContent getSimpleContent(SimpleContentType type)
{
	const auto config = getSimpleContentConfig(type);

	SimpleContentBuilder builder{};
	builder.setConfig(config);
	return builder.build();
}

SimpleContent createSimpleContent(const std::string& name)
{
	auto builder = SimpleContentBuilder{};

	if (name == "multiple_objects")
	{
		builder.setType(SimpleContentType::SQUARE);
		builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
		builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "multiple_objects.frag");

		SimpleContent content = builder.build();
		content.setUniform("u_light", glm::vec3(1.0, 1.0, 1.0));
		content.setUniform("u_diffuse", glm::vec3(1.0, 1.0, 0.0));
		content.setUniform("u_refl_exp", 30.0f);
		content.setUniform("u_clamp_diffuse", 0.5f);
		return content;
	}
	else if (name == "cyclope")
	{
		builder.setType(SimpleContentType::SQUARE);
		builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
		builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "plain_texture.frag");
		builder.setTexturePath(pathToModels / "cyclope.png");

		SimpleContent content = builder.build();
		return content;
	}
	else if (name == "seascape")
	{
		builder.setType(SimpleContentType::SQUARE);
		builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
		builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "seascape.frag");

		SimpleContent content = builder.build();
		content.setUniform("u_fresnel_exp", 3.0F);
		return content;
	}
	else if (name == "offscreen")
	{
		builder.setType(SimpleContentType::SQUARE);
		builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
		builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "offscreen_tex.frag");

		SimpleContent content = builder.build();
		content.setUniform("u_fresnel_exp", 3.0F);
		return content;
	}
	else
	{
		builder.setType(SimpleContentType::SQUARE);
		builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
		builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "multiple_objects.frag");
		builder.setTexturePath(pathToModels / "cyclope.png");

		SimpleContent content = builder.build();
		return content;
	}
}



void SimpleContentBuilder::setConfig(const SimpleContentConfig& config)
{
	m_config = config;
}

SimpleContent SimpleContentBuilder::build()
{
	SimpleContent content = SimpleContent(m_config.m_vertices, m_config.m_indices, m_config.m_mode, m_config.m_vertexShaderPath, m_config.m_fragmentShaderPath,
		m_config.m_texturePath, m_config.m_uniforms, m_config.m_anyUniforms);
	content.setMV(m_config.m_MVDefault);
	for (const auto& [uniform, value] : m_config.m_uniformDefaults)
	{
		content.setUniform(uniform, value);
	}
	return content;
}

void SimpleContentBuilder::setType(const SimpleContentType type)
{
	m_config = getSimpleContentConfig(type);
}

void SimpleContentBuilder::setVertexShaderPath(const fs::path& vertexShaderPath)
{
	m_config.m_vertexShaderPath = vertexShaderPath;
}

void SimpleContentBuilder::setFragmentShaderPath(const fs::path& fragmentShaderPath)
{
	m_config.m_fragmentShaderPath = fragmentShaderPath;
}

void SimpleContentBuilder::setTexturePath(const fs::path& texturePath)
{
	m_config.m_texturePath = texturePath;
}

void SimpleContentBuilder::setVertices(const std::vector<glm::vec3>& vertices)
{
	m_config.m_vertices = vertices;
}

void SimpleContentBuilder::setIndices(std::vector<unsigned short>& indices)
{
	m_config.m_indices = indices;
}

void SimpleContentBuilder::setUniforms(const std::unordered_set<std::string>& uniforms)
{
	m_config.m_uniforms = uniforms;
}

