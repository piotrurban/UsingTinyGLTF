#pragma once
#include <GL/glew.h>
#include <string>

bool LoadShader(GLenum shaderType, GLuint& shader, const char* shaderSourceFilename);
bool LinkShader(GLuint& prog, GLuint& vertShader, GLuint& fragShader);
void CheckErrors(std::string desc);

size_t ComponentTypeByteSize(int type);
