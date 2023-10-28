#pragma once
#include "gl_includes.h"
#include <string>
#include <vector>
#include <chrono>
#include <Content.h>
using namespace std::chrono_literals;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define CheckGLErrors(desc)                                                   \
  {                                                                           \
    GLenum e = glGetError();                                                  \
    if (e != GL_NO_ERROR) {                                                   \
      printf("OpenGL error in \"%s\": %d (%d) %s:%d\n", desc, e, e, __FILE__, \
             __LINE__);                                                       \
      exit(20);                                                               \
    }                                                                         \
  }


bool LoadShader(GLenum shaderType, GLuint& shader, const char* shaderSourceFilename);
bool LinkShader(GLuint& prog, GLuint& vertShader, GLuint& fragShader);
void CheckErrors(std::string desc);

size_t ComponentTypeByteSize(int type);
void QuatToAngleAxis(const std::vector<double> quaternion,
	double& outAngleDegrees,
	double* axis);

bool getPeriodicSignal(std::chrono::milliseconds period);

glm::dmat4 getContentMVP(const Content& content);

unsigned char getTypeSize(int type);

void traverseModelMeshes(const Content& content);
glm::mat4 getNodeMVP(const Content& content, unsigned short id);
void traverseNode(const Content& content, const tinygltf::Node& node, std::vector<std::string> path, const glm::mat4& mvp);

