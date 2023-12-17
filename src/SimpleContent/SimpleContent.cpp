#include "SimpleContent.h"
#include "utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <string>

namespace fs = std::filesystem;
using namespace std::literals;

const fs::path SimpleContent::s_defaultVertexShader{ fs::path{__FILE__}.parent_path() / "shaders/simple.vert" };
const fs::path SimpleContent::s_defaultFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple.frag" };
const fs::path SimpleContent::s_circleFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple_circle.frag" };
const fs::path SimpleContent::s_distanceFragmentShader{ fs::path{__FILE__}.parent_path() / "shaders/simple_distance.frag" };

SimpleContent::SimpleContent(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices, GLenum mode,
	const std::optional<std::filesystem::path> vertexShaderPath,
	const std::optional<std::filesystem::path> fragmentShaderPath,
	const std::unordered_set<std::string>& uniforms,
	const std::unordered_set<std::string>& anyUniforms
)
	: m_vertices{ vertices }
	, m_indices{ indices }
	, m_mode{ mode }
	, m_vertexShaderPath{ vertexShaderPath }
	, m_fragmentShaderPath{ fragmentShaderPath }
	, m_vertexBuffer{ 0 }
	, m_indicesBuffer{ 0 }
	, m_vertexArray{ 0 }
	, m_indexArray{ 0 }
	, m_prog{ 0 }
	, m_uniformMap{}
	, m_anyUniformMap{}
{
	for (const std::string& uniform : uniforms)
	{
		m_uniformMap[uniform] = 0.0F;
	}
	for (const std::string& anyUniform : anyUniforms)
	{
		m_anyUniformMap[anyUniform] = std::any{};
	}
	setupVertices();
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
}

void SimpleContent::setMV(const glm::mat4& mv)
{
	m_mv = mv;
}

void SimpleContent::setUniform(const std::string& name, const float value)
{
	m_uniformMap[name] = value;
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

	/*GLuint u_radius = glGetUniformLocation(m_prog, "u_radius");
	CheckErrors("get radius loc");
	if (u_radius != -1)
	{
		glUniform1f(u_radius, 0.3F);
	}*/

	for (const auto& [uniform, value] : m_uniformMap)
	{
		glUniform1f(m_uniformLoc.at(uniform), value);
		CheckErrors("set uniform "s + uniform);
	}
	/*for (const auto& [uniform, loc] : m_uniformLoc)
	{
		glUniform1f(loc, m_uniformMap.at(uniform));
		CheckErrors("set uniform "s + uniform);
	}*/

	for (const auto& [uniform, value] : m_anyUniformMap)
	{
		if (value.type() == typeid(glm::mat4))
		{
			glUniformMatrix4fv(m_uniformLoc.at(uniform), 1, GL_FALSE, glm::value_ptr(std::any_cast<glm::mat4>(value)));
			CheckErrors("set any uniform (mat4) "s + uniform);
		}
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