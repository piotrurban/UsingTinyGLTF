#pragma once
#include <GL/glew.h>
#include <tiny_gltf.h>

#include "utils.h"

#include <string>
#include <vector>

typedef struct {
	GLuint vb;
} GLBufferState;

typedef struct {
	std::vector<GLuint> diffuseTex;  // for each primitive in mesh
} GLMeshState;

typedef struct {
	std::map<std::string, GLint> attribs;
	std::map<std::string, GLint> uniforms;
} GLProgramState;

typedef struct {
	GLuint vb;     // vertex buffer
	size_t count;  // byte count
} GLCurvesState;

class Content
{
public:
	Content(const std::string& gltf_file, const std::string& vertex_shader_file, const std::string& fragment_shader_file);
	bool load_from_file(const std::string& filename);
	bool load_and_link_shaders_from_files(const std::string& vertex_shader, const std::string& fragment_shader);
	void setup_mesh_data();

	tinygltf::Model m_model;
	std::map<int, GLBufferState> m_bufferState;
	std::map<std::string, GLMeshState> m_meshState;
	std::map<int, GLCurvesState> m_curvesMesh;
	std::string m_vertex_shader_name;
	std::string m_fragment_shader_name;
	GLProgramState m_GLProgramState;
	GLuint m_vertId;
	GLuint m_fragId;
	GLuint m_progId;
};