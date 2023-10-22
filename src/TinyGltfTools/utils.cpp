#include "utils.h"
#include <tiny_gltf.h>

#include <vector>
#include <cassert>

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