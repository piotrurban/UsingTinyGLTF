#pragma once
#include "gl_includes.h"
#include <tiny_gltf.h>

#include <string>
#include <map>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>


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

	std::map<unsigned short, std::vector<glm::vec3>>& getMeshToPositionsMap();


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

	glm::dmat4 m_perspectiveMat;
	glm::dmat4 m_viewMat;
	glm::dmat4 m_modelMat;

	std::map<unsigned short, std::vector<glm::vec3>> m_meshToPositionMap;
};

struct MeshDataBufferView
{
	MeshDataBufferView() = default;
	MeshDataBufferView(const unsigned char* rawData, size_t count, unsigned long long stride, int type, int componentType);
	const inline unsigned char* operator[](unsigned long long index) const
	{
		return m_rawData + index * m_stride;
	}

	const unsigned char* m_rawData{ nullptr };
	size_t m_count;
	unsigned long long  m_stride;
	int m_type;
	int m_componentType;
	static const MeshDataBufferView s_empty;
};

glm::vec3 getVec3(const MeshDataBufferView& dataView, unsigned long long index);

const unsigned short getIndex(const MeshDataBufferView& dataView, unsigned long long index);

MeshDataBufferView getMeshAttributeData(const Content& content, const unsigned short meshIndex, const char* attributeName);

std::vector<glm::vec3> generateMeshPositions(const Content& content, unsigned short meshIndex);

std::map<unsigned short, std::vector<glm::vec3>>& getMeshToPositionsMap(const Content& content);

