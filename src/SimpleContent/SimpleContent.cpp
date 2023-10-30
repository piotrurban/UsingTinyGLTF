#include "SimpleContent.h"
#include "utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>

void SimpleContent::setupVertices(const std::vector<glm::vec3>& vertices, const std::vector<unsigned short>& indices)
{
	m_vertices = vertices;

	glGenBuffers(1, &m_vertexBuffer);
	CheckErrors("gen vertex buffer");
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	CheckErrors("bind buffer");
	const auto dataptr = glm::value_ptr(m_vertices.at(0));
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(float), glm::value_ptr(m_vertices.at(0)), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_indices = indices;
	glGenBuffers(1, &m_indicesBuffer);
	CheckErrors("gen indices buffer");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer);
	CheckErrors("bind indices buffer");
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned short), m_indices.data(), GL_STATIC_DRAW);
	CheckErrors("indices buffer data");
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	CheckErrors("unbind element buffer");
}

void SimpleContent::loadShaders()
{
	const std::filesystem::path vertexShaderPath = std::filesystem::path{ __FILE__ }.parent_path() / "shaders/simple.vert";
	const std::filesystem::path fragmentShaderPath = std::filesystem::path{ __FILE__ }.parent_path() / "shaders/simple.frag";
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
}

void SimpleContent::setMV(const glm::mat4& mv)
{
	m_mv = mv;
}

void SimpleContent::draw()
{
	glUseProgram(m_prog);
	CheckErrors("use program");
	GLuint u_MVP = glGetUniformLocation(m_prog, "u_MVP");
	CheckErrors("get mvp loc");
	glUniformMatrix4fv(u_MVP, 1, GL_FALSE, glm::value_ptr(m_mv));
	CheckErrors("set mvp");

	GLuint vertArray;
	glGenVertexArrays(1, &vertArray);
	CheckErrors("gen vert array");
	glBindVertexArray(vertArray);
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

	glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, nullptr);
	CheckErrors("draw elements");

	glDisableVertexAttribArray(posLoc);
	CheckErrors("disable vertex attrib 0");
}

