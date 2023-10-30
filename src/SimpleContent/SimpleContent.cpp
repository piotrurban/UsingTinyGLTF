#include "SimpleContent.h"
#include "utils.h"
#include <fstream>
#include <filesystem>
#include <iostream>

void SimpleContent::setupVertices(const std::vector<glm::vec3>& vertices)
{
	m_vertices = vertices;
	if (m_vertexBuffer == 0)
	{
		glGenBuffers(1, &m_vertexBuffer);
		CheckErrors("gen buffers");
	}
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	CheckErrors("bind buffer");
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * 3 * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SimpleContent::loadShaders()
{
	const std::filesystem::path vertexShaderPath = std::filesystem::path{ __FILE__ }.parent_path() / "shaders/simple.vert";
	const std::filesystem::path fragmentShaderPath = std::filesystem::path{ __FILE__ }.parent_path() / "shaders/simple.frag";
	std::ifstream vertexIFS{ vertexShaderPath, std::ios::binary|std::ios::in };
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

}

void SimpleContent::setMV(const glm::mat4& mv)
{
	m_mv = mv;
}

void SimpleContent::draw()
{

}
