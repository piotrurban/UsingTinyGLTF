#include "utils.h"
#include <tiny_gltf.h>

#include <vector>
#include <cassert>
#include <iostream>

extern std::map<unsigned short, std::vector<glm::vec3>> nodeToMeshPositionsMap;

GLenum glCheckError_(const char* file, int line, GLenum errorCode)
{
	//GLenum errorCode;
	if (errorCode != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
		case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError(e) glCheckError_(__FILE__, __LINE__, e) 

bool LoadShader(GLenum shaderType,  // GL_VERTEX_SHADER or GL_FRAGMENT_SHADER(or
	// maybe GL_COMPUTE_SHADER)
	GLuint& shader, const char* shaderSourceFilename) {
	GLint val = 0;

	// free old shader/program
	if (shader != 0) {
		glDeleteShader(shader);
	}

	std::vector<GLchar> srcbuf;
	FILE* fp = fopen(shaderSourceFilename, "rb");
	if (!fp) {
		fprintf(stderr, "failed to load shader: %s\n", shaderSourceFilename);
		return false;
	}
	fseek(fp, 0, SEEK_END);
	size_t len = ftell(fp);
	rewind(fp);
	srcbuf.resize(len + 1);
	len = fread(&srcbuf.at(0), 1, len, fp);
	srcbuf[len] = 0;
	fclose(fp);

	const GLchar* srcs[1];
	srcs[0] = &srcbuf.at(0);

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, srcs, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &val);
	if (val != GL_TRUE) {
		char log[4096];
		GLsizei msglen;
		glGetShaderInfoLog(shader, 4096, &msglen, log);
		printf("%s\n", log);
		// assert(val == GL_TRUE && "failed to compile shader");
		printf("ERR: Failed to load or compile shader [ %s ]\n",
			shaderSourceFilename);
		return false;
	}

	printf("Load shader [ %s ] OK\n", shaderSourceFilename);
	return true;
}

bool LinkShader(GLuint& prog, GLuint& vertShader, GLuint& fragShader) {
	GLint val = 0;

	if (prog != 0) {
		glDeleteProgram(prog);
	}

	prog = glCreateProgram();

	glAttachShader(prog, vertShader);
	glAttachShader(prog, fragShader);
	glLinkProgram(prog);

	glGetProgramiv(prog, GL_LINK_STATUS, &val);
	assert(val == GL_TRUE && "failed to link shader");

	printf("Link shader OK\n");

	return true;
}

void CheckErrors(std::string desc) {
	GLenum e = glGetError();
	if (e != GL_NO_ERROR) {
		fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
		glCheckError(e);
		exit(20);
	}
}

size_t ComponentTypeByteSize(int type) {
	switch (type) {
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
	case TINYGLTF_COMPONENT_TYPE_BYTE:
		return sizeof(char);
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
	case TINYGLTF_COMPONENT_TYPE_SHORT:
		return sizeof(short);
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
	case TINYGLTF_COMPONENT_TYPE_INT:
		return sizeof(int);
	case TINYGLTF_COMPONENT_TYPE_FLOAT:
		return sizeof(float);
	case TINYGLTF_COMPONENT_TYPE_DOUBLE:
		return sizeof(double);
	default:
		return 0;
	}
}

void QuatToAngleAxis(const std::vector<double> quaternion,
	double& outAngleDegrees,
	double* axis) {
	double qx = quaternion[0];
	double qy = quaternion[1];
	double qz = quaternion[2];
	double qw = quaternion[3];

	double angleRadians = 2 * acos(qw);
	if (angleRadians == 0.0) {
		outAngleDegrees = 0.0;
		axis[0] = 0.0;
		axis[1] = 0.0;
		axis[2] = 1.0;
		return;
	}

	double denom = sqrt(1 - qw * qw);
	outAngleDegrees = angleRadians * 180.0 / 3.1415926;
	axis[0] = qx / denom;
	axis[1] = qy / denom;
	axis[2] = qz / denom;
}

bool getPeriodicSignal(std::chrono::milliseconds period)
{
	static auto last_time = std::chrono::steady_clock::now();
	const auto now = std::chrono::steady_clock::now();
	if (now - last_time > period)
	{
		last_time = now;
		return true;
	}
	return false;
}

glm::dmat4 getContentMVP(const Content& content)
{
	return content.m_perspectiveMat * content.m_viewMat * content.m_modelMat;
}

unsigned char getTypeSize(int type)
{
	unsigned char size{};
	if (type == TINYGLTF_TYPE_SCALAR) {
		size = 1;
	}
	else if (type == TINYGLTF_TYPE_VEC2) {
		size = 2;
	}
	else if (type == TINYGLTF_TYPE_VEC3) {
		size = 3;
	}
	else if (type == TINYGLTF_TYPE_VEC4) {
		size = 4;
	}
	else {
		assert(0);
	}
	return size;
}

void traverseModelMeshes(const Content& content)
{
	std::vector<std::string> nodePath{};
	glm::mat4 mvp = glm::mat4(1.0);
	for (const auto& node : content.m_model.nodes)
	{
		traverseNode(content, node, nodePath, mvp);
	}
}

void traverseNode(const Content& content, const tinygltf::Node& node, std::vector<std::string> path, const glm::mat4& mvp)
{
	path.push_back(node.name);
	std::cout << "Traverse nodes: " << node.name << "\n";
	if (node.name == "Cube.001" && node.mesh >= 0)
	{
		std::cout << "mesh " << node.name << " positions:\n";
		const auto& positions = nodeToMeshPositionsMap.at(node.mesh);
		const glm::mat4 modelMVP = getContentMVP(content);
		for (const glm::vec3& vertex : positions)
		{
			const glm::vec4 trafoVertex =  modelMVP * mvp * glm::vec4(vertex, 1.0F);
			std::cout << std::format("vec {}, {}, {}\n", trafoVertex.x/trafoVertex.w, trafoVertex.y/trafoVertex.w, trafoVertex.z/trafoVertex.w);
		}
	}
	for (int id : node.children)
	{
		const tinygltf::Node& childNode = content.m_model.nodes.at(id);
		const glm::mat4 local_mvp{ getNodeMVP(content, id) };
		traverseNode(content, childNode, path, mvp * glm::mat4(local_mvp));
	}
}



glm::mat4 getNodeMVP(const Content& content, unsigned short id)
{
	const tinygltf::Node& node = content.m_model.nodes.at(id);
	glm::dmat4 result{ glm::dmat4(1.0F) };
	if (node.translation.size() == 3) {

		result = glm::translate(result, glm::make_vec3(node.translation.data()));
	}

	if (node.rotation.size() == 4) {
		double angleDegrees;
		double axis[3];

		QuatToAngleAxis(node.rotation, angleDegrees, axis);

		glRotated(angleDegrees, axis[0], axis[1], axis[2]);
		result = glm::rotate(result, angleDegrees * 3.1415926 / 180.0, glm::make_vec3(node.rotation.data()));
	}

	if (node.scale.size() == 3) {
		glScaled(node.scale[0], node.scale[1], node.scale[2]);
		result = glm::scale(result, glm::make_vec3(node.scale.data()));
	}
	return result;
}