#include "SimpleContent.h"
#include <stb_image.h>

#include "utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>
#include <random>
namespace fs = std::filesystem;
using namespace std::literals;

const fs::path SimpleContent::s_defaultVertexShader{ fs::path{__FILE__}.parent_path() / "shaders/simple.vert" };
const fs::path SimpleContent::s_defaultFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple.frag" };
const fs::path SimpleContent::s_circleFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple_circle.frag" };
const fs::path SimpleContent::s_distanceFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple_distance.frag" };

SimpleContent::SimpleContent(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices, GLenum mode,
	const std::optional<std::filesystem::path> vertexShaderPath,
	const std::optional<std::filesystem::path> fragmentShaderPath,
	const std::optional<std::filesystem::path> texturePath,
	const std::unordered_set<std::string>& uniforms,
	const std::unordered_map<std::string, std::any>& anyUniforms
)
	: m_vertices{ vertices }
	, m_indices{ indices }
	, m_mode{ mode }
	, m_vertexShaderPath{ vertexShaderPath }
	, m_fragmentShaderPath{ fragmentShaderPath }
	, m_texturePath{ texturePath }
	, m_vertexBuffer{ 0 }
	, m_indicesBuffer{ 0 }
	, m_vertexArray{ 0 }
	, m_indexArray{ 0 }
	, m_tex{ 0 }
	, m_prog{ 0 }
	, m_uniformMap{}
	, m_anyUniformMap{ anyUniforms }
	, m_uniformTextureSamplerLoc{ -1 }
	, m_fbo1{ 0 }
	, m_fbo2{ 0 }
	, m_tex1{ 0 }
	, m_tex2{ 0 }
{
	for (const std::string& uniform : uniforms)
	{
		m_uniformMap[uniform] = 0.0F;
	}
	setupVertices();
	setupOffscreenBuffers();
	loadShaders();
}

void SimpleContent::setupVertices()
{

	glGenBuffers(1, &m_vertexBuffer);
	CheckErrors("gen vertex buffer");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	CheckErrors("bind buffer");
	const auto dataptr = glm::value_ptr(m_vertices.at(0));
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(float), glm::value_ptr(m_vertices.at(0)), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &m_indicesBuffer);
	CheckErrors("gen indices buffer");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	CheckErrors("bind indices buffer");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), m_indices.data(), GL_STATIC_DRAW);
	CheckErrors("indices buffer data");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CheckErrors("unbind element buffer");

	glGenVertexArrays(1, &m_vertexArray);
	CheckErrors("gen vert array");

	if (m_texturePath && m_texturePath != "")
	{
		glGenTextures(1, &m_tex);
		CheckErrors("gen textures");
		unsigned char* tex_data{ nullptr };
		int x, y, n;
		tex_data = stbi_load(m_texturePath.value().string().c_str(), &x, &y, &n, 0);
		if (tex_data)
		{
			glBindTexture(GL_TEXTURE_2D, m_tex);
			CheckErrors("bind 2Dtex");
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
			CheckErrors("glTexImage2D");
			glGenerateMipmap(GL_TEXTURE_2D);
			CheckErrors("gen mipmap");
			glBindTexture(GL_TEXTURE_2D, 0);
			CheckErrors("unbind tex2D");
			stbi_image_free(tex_data);
		}
	}
}

void SimpleContent::loadShaders()
{
	const std::filesystem::path vertexShaderPath = m_vertexShaderPath ? *m_vertexShaderPath : s_defaultVertexShader;
	const std::filesystem::path fragmentShaderPath = m_fragmentShaderPath ? *m_fragmentShaderPath : s_defaultFragmentShader;
	std::ifstream vertexIFS{ vertexShaderPath, std::ios::binary | std::ios::in };
	if (!vertexIFS.good())
	{
		throw std::exception((std::string("vertex shader file not found ") + vertexShaderPath.string()).c_str());
	}
	vertexIFS.seekg(0, std::ios::end);
	const int len = vertexIFS.tellg();
	std::unique_ptr<char[]> buf = std::make_unique<char[]>(len + 1U);
	vertexIFS.seekg(0, std::ios::beg);
	vertexIFS.read(buf.get(), len);
	vertexIFS.close();

	buf[len] = 0;
	std::cout << buf << std::endl;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	CheckErrors("create vertex shader");
	const char* vertexSource = buf.get();
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	CheckErrors("vertex source");
	glCompileShader(vertexShader);
	CheckErrors("compile vertex shader");
	GLint success;
	GLchar info[10000];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 10000, nullptr, info);
		std::cerr << "Vertex shader compilation failed. Log:\n" << info << std::endl;
	}

	std::ifstream fragmentIFS{ fragmentShaderPath, std::ios::binary | std::ios::in };
	if (!fragmentIFS.good())
	{
		throw std::exception((std::string("fragment shader file not found ") + fragmentShaderPath.string()).c_str());
	}
	fragmentIFS.seekg(0, std::ios::end);
	const int fraglen = fragmentIFS.tellg();
	std::unique_ptr<char[]> fragbuf = std::make_unique<char[]>(fraglen + 1U);
	fragmentIFS.seekg(0, std::ios::beg);
	fragmentIFS.read(fragbuf.get(), fraglen);
	fragmentIFS.close();

	fragbuf[fraglen] = 0;
	std::cout << fragbuf << std::endl;
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	CheckErrors("create fragment shader");
	const char* fragmentSource = fragbuf.get();
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	CheckErrors("fragment source");
	glCompileShader(fragmentShader);
	CheckErrors("compile fragment shader");
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, sizeof(info), nullptr, info);
		std::cerr << "Fragment shader compilation failed. Log:\n" << info << std::endl;
	}

	m_prog = glCreateProgram();
	CheckErrors("create program");
	glAttachShader(m_prog, vertexShader);
	CheckErrors("attach vertex");
	glAttachShader(m_prog, fragmentShader);
	CheckErrors("attach fragment");
	glLinkProgram(m_prog);
	CheckErrors("link prog");
	GLint val;
	glGetProgramiv(m_prog, GL_LINK_STATUS, &val);
	assert(val == GL_TRUE && "failed to link shader");

	m_uniforms = getShaderUniforms();
	setAnyUniformsFromShaderCode();

	for (const auto& [uniform, _] : m_uniformMap)
	{
		m_uniformLoc[uniform] = glGetUniformLocation(m_prog, uniform.c_str());
		CheckErrors(std::string("get uniform loc ") + uniform);
		if (m_uniformLoc.at(uniform) == -1)
		{
			std::cerr << std::format("WARNING: uniform {} was not located in shaders.\n", uniform);
		}
	}
	for (const auto& [uniform, _] : m_anyUniformMap)
	{
		m_uniformLoc[uniform] = glGetUniformLocation(m_prog, uniform.c_str());
		CheckErrors(std::string("get uniform loc ") + uniform);
		if (m_uniformLoc.at(uniform) == -1)
		{
			std::cerr << std::format("WARNING: uniform {} was not located in shaders.\n", uniform);
		}
	}
	m_uniformTextureSamplerLoc = glGetUniformLocation(m_prog, "u_textureSampler");
	setUniform("u_textureSampler", 0);
}

void SimpleContent::setMV(const glm::mat4& mv)
{
	m_mv = mv;
}

void SimpleContent::setUniform(const std::string& name, const std::any value)
{
	m_anyUniformMap[name] = value;
}

void SimpleContent::draw()
{
	glUseProgram(m_prog);
	CheckErrors("use program");
	GLuint u_MVP = glGetUniformLocation(m_prog, "u_MVP");
	CheckErrors("get mvp loc");
	glUniformMatrix4fv(u_MVP, 1, GL_FALSE, glm::value_ptr(m_mv));
	CheckErrors("set mvp");

	for (auto& [name, type] : m_uniforms)
	{
		switch (type)
		{
		case GL_INT:
		{
			glUniform1i(m_uniformLoc.at(name), std::any_cast<int>(m_anyUniformMap.at(name)));
			CheckErrors("set uniform "s + name);
			break;
		}
		case GL_UNSIGNED_INT:
		{
			glUniform1ui(m_uniformLoc.at(name), std::any_cast<uint32_t>(m_anyUniformMap.at(name)));
			CheckErrors("set uniform "s + name);
			break;
		}
		case GL_FLOAT:
		{
			glUniform1f(m_uniformLoc.at(name), std::any_cast<float>(m_anyUniformMap.at(name)));
			CheckErrors("set uniform "s + name);
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glUniform3fv(m_uniformLoc.at(name), 1, glm::value_ptr(std::any_cast<glm::vec3>(m_anyUniformMap.at(name))));
			CheckErrors("set any uniform (vec3"s + name);
			break;
		}
		case GL_FLOAT_VEC4:
		{
			glUniform4fv(m_uniformLoc.at(name), 1, glm::value_ptr(std::any_cast<glm::vec4>(m_anyUniformMap.at(name))));
			CheckErrors("set any uniform (vec4"s + name);
			break;
		}
		case GL_FLOAT_MAT4:
		{
			glUniformMatrix4fv(m_uniformLoc.at(name), 1, GL_FALSE, glm::value_ptr(std::any_cast<glm::mat4>(m_anyUniformMap.at(name))));
			CheckErrors("set any uniform (mat4) "s + name);
			break;
		}
		case GL_SAMPLER_2D:
		{
			glUniform1i(m_uniformLoc.at(name), std::any_cast<int32_t>(m_anyUniformMap.at(name)));
			CheckErrors("set any uniform (sampler2D) "s + name);
			break;
		}
		default:
			break;
		}
	}

	if (m_uniformTextureSamplerLoc != -1)
	{
		glUniform1i(m_uniformTextureSamplerLoc, 0);
		CheckErrors("set texture uniform");
		glActiveTexture(GL_TEXTURE0);
		CheckErrors("active tex");
		glBindTexture(GL_TEXTURE_2D, m_tex);
	}

	glBindVertexArray(m_vertexArray);
	CheckErrors("bind vert array");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	CheckErrors("bind vert buffer");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	CheckErrors("bind indices buffer");

	GLint posLoc = glGetAttribLocation(m_prog, "in_vertex");
	glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	CheckErrors("vertex attrib ptr");
	glEnableVertexAttribArray(posLoc);
	CheckErrors("enable vertex attrib 0");

	glDrawElements(m_mode, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
	CheckErrors("draw elements");

	glDisableVertexAttribArray(posLoc);
	CheckErrors("disable vertex attrib 0");
}

SimpleContent::~SimpleContent()
{
}

void SimpleContent::cleanup()
{
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indicesBuffer);
	glDeleteVertexArrays(1, &m_vertexArray);
}

std::vector<std::pair<std::string, GLenum>>  SimpleContent::getShaderUniforms()
{
	GLint count;
	glGetProgramiv(m_prog, GL_ACTIVE_UNIFORMS, &count);
	CheckErrors("get active uniform count");
	std::vector<std::pair<std::string, GLenum>> uniforms{};
	GLchar name[256];
	GLsizei length;
	GLint size;
	GLenum type;

	for (int i = 0; i < count; i++)
	{
		glGetActiveUniform(m_prog, i, sizeof(name), &length, &size, &type, name);
		CheckErrors("get active uniform");
		uniforms.push_back(std::make_pair(name, type));
	}

	return uniforms;
}

void SimpleContent::setAnyUniformsFromShaderCode()
{
	const std::unordered_map<GLenum, std::any> GLTypeToCXX{
		{GL_FLOAT, std::any{0.0F}},
		{GL_INT, std::any{0}},
		{GL_UNSIGNED_INT, std::any{0U}},
		{GL_FLOAT_VEC3, std::any{glm::vec3{}}},
		{GL_FLOAT_VEC4, std::any{glm::vec4{}}},
		{GL_FLOAT_MAT4, std::any{glm::mat4{}}},
		{GL_SAMPLER_2D, std::any{0U}}
	};

	for (auto& [name, type] : m_uniforms)
	{
		if ((GLTypeToCXX.count(type) == 1) && (m_anyUniformMap.count(name) == 0))
		{
			m_anyUniformMap[name] = GLTypeToCXX.at(type);
		}
	}
}

void SimpleContent::setupOffscreenBuffers()
{
	std::array<GLuint, 2> textures = { 0U,0U };
	unsigned char* pix = new unsigned char[400];

	std::mt19937 rng;
	std::random_device rd;
	std::generate_n(pix, 400, std::ref(rd));
	for (GLuint& tex : textures)
	{
		std::generate_n(pix, 400, std::ref(rd));
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 10, 10, 0, GL_RGB, GL_UNSIGNED_BYTE, pix);
		CheckErrors("tex 2D");
	}
	m_tex1 = textures.at(0);
	m_tex2 = textures.at(1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_tex1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_tex2);
	
	std::array<GLuint, 2> fbos = { 0U,0U };
	for (uint8_t i = 0; i < fbos.size(); i++)
	{
		glGenFramebuffers(1, &fbos[i]);
		CheckErrors("framebuf");
		glBindFramebuffer(GL_FRAMEBUFFER, fbos[i]);
		CheckErrors("framebuf bind");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);
		if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
		{
			std::cerr << "Failed to initialize framebuffer " << fbos[i] << std::endl;
		}
	}


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	m_fbo1 = fbos.at(0);
	m_fbo2 = fbos.at(1);
	delete[] pix;
}