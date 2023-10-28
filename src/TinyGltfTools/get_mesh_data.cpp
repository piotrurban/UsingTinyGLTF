#include "get_mesh_data.h"

#include <TransformUtils.h>
#include <utils.h>

#include <glm/vec2.hpp>

#include <iostream>
void get_mesh_data(Content& content, tinygltf::Mesh& mesh)
{
	tinygltf::Model& model = content.m_model;
	//// Skip curves primitive.
 // if (gCurvesMesh.find(mesh.name) != gCurvesMesh.end()) {
 //  return;
 //}

 // if (content.m_GLProgramState.uniforms["diffuseTex"] >= 0) {
 //  glUniform1i(content.m_GLProgramState.uniforms["diffuseTex"], 0);  // TEXTURE0
 //}

	if (content.m_GLProgramState.uniforms["isCurvesLoc"] >= 0) {
		glUniform1i(content.m_GLProgramState.uniforms["isCurvesLoc"], 0);
	}

	for (size_t i = 0; i < mesh.primitives.size(); i++) {
		const tinygltf::Primitive& primitive = mesh.primitives[i];

		if (primitive.indices < 0) return;

		// Assume TEXTURE_2D target for the texture object.
		// glBindTexture(GL_TEXTURE_2D, gMeshState[mesh.name].diffuseTex[i]);

		std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
		std::map<std::string, int>::const_iterator itEnd(
			primitive.attributes.end());

		auto positionAttribIt = std::find_if(it, itEnd, [](auto& attrib) {return attrib.first.compare("POSITION") == 0; });
		if (positionAttribIt != itEnd)
		{
			const tinygltf::Accessor& accessor = model.accessors[positionAttribIt->second];
			content.m_bufferState[accessor.bufferView];
			GLint bufferSize{};
			GLuint bufferName{ content.m_bufferState[accessor.bufferView].vb };
			glGetNamedBufferParameteriv(bufferName, GL_BUFFER_SIZE, &bufferSize);
			std::vector<float> float_data(bufferSize / sizeof(float));

			if (primitive.mode == TINYGLTF_MODE_TRIANGLES)
			{
				const auto& index_accessor = model.accessors[primitive.indices];
				std::vector<unsigned short> indices(index_accessor.count);
				glGetNamedBufferSubData(content.m_bufferState[index_accessor.bufferView].vb, index_accessor.byteOffset, index_accessor.count * 2,
					indices.data());
				std::vector<glm::vec3> triangle_data(bufferSize / sizeof(float) / 3);
				glGetNamedBufferSubData(bufferName, 0, bufferSize, triangle_data.data());
				const glm::mat4 curr_mvp = getContentMVP(content);
				std::cout << "triangle mesh data: NAME = " << mesh.name << ", SIZE = 3 * " << triangle_data.size() <<
					", DATA: "
					<< curr_mvp * glm::vec4(triangle_data[indices[0]],1.0)
					<< ", "
					<< curr_mvp * glm::vec4(triangle_data[indices[1]], 1.0)
					<< "...\n";
			}
			else
			{
				glGetNamedBufferSubData(bufferName, 0, bufferSize, float_data.data());
				std::cout << "mesh data: NAME = " << mesh.name << ", SIZE = " << float_data.size() << ", DATA: " << float_data[0] << ", " << float_data[1] << "...\n";
			}
		}
		//for (; it != itEnd; it++) {
		//	assert(it->second >= 0);
		//	const tinygltf::Accessor& accessor = model.accessors[it->second];
		//	glBindBuffer(GL_ARRAY_BUFFER, content.m_bufferState[accessor.bufferView].vb);
		//	CheckErrors("bind buffer");
		//	int size = 1;
		//	if (accessor.type == TINYGLTF_TYPE_SCALAR) {
		//		size = 1;
		//	}
		//	else if (accessor.type == TINYGLTF_TYPE_VEC2) {
		//		size = 2;
		//	}
		//	else if (accessor.type == TINYGLTF_TYPE_VEC3) {
		//		size = 3;
		//	}
		//	else if (accessor.type == TINYGLTF_TYPE_VEC4) {
		//		size = 4;
		//	}
		//	else {
		//		assert(0);
		//	}
		//	// it->first would be "POSITION", "NORMAL", "TEXCOORD_0", ...
		//	if ((it->first.compare("POSITION") == 0) ||
		//		(it->first.compare("NORMAL") == 0) ||
		//		(it->first.compare("TEXCOORD_0") == 0)) {
		//		if (content.m_GLProgramState.attribs[it->first] >= 0) {
		//			// Compute byteStride from Accessor + BufferView combination.
		//			int byteStride =
		//				accessor.ByteStride(model.bufferViews[accessor.bufferView]);
		//			assert(byteStride != -1);
		//			glVertexAttribPointer(content.m_GLProgramState.attribs[it->first], size,
		//				accessor.componentType,
		//				accessor.normalized ? GL_TRUE : GL_FALSE,
		//				byteStride, BUFFER_OFFSET(accessor.byteOffset));
		//			CheckErrors("vertex attrib pointer");
		//			glEnableVertexAttribArray(content.m_GLProgramState.attribs[it->first]);
		//			CheckErrors("enable vertex attrib array");
		//		}
		//	}
		//}

		//const tinygltf::Accessor& indexAccessor =
		//	model.accessors[primitive.indices];
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
		//	content.m_bufferState[indexAccessor.bufferView].vb);
		//CheckErrors("bind buffer");
		//int mode = -1;
		//if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
		//	mode = GL_TRIANGLES;
		//}
		//else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
		//	mode = GL_TRIANGLE_STRIP;
		//}
		//else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
		//	mode = GL_TRIANGLE_FAN;
		//}
		//else if (primitive.mode == TINYGLTF_MODE_POINTS) {
		//	mode = GL_POINTS;
		//}
		//else if (primitive.mode == TINYGLTF_MODE_LINE) {
		//	mode = GL_LINES;
		//}
		//else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
		//	mode = GL_LINE_LOOP;
		//}
		//else {
		//	assert(0);
		//}
		//glDrawElements(mode, indexAccessor.count, indexAccessor.componentType,
		//	BUFFER_OFFSET(indexAccessor.byteOffset));
		//CheckErrors("draw elements");

		//{
		//	std::map<std::string, int>::const_iterator it(
		//		primitive.attributes.begin());
		//	std::map<std::string, int>::const_iterator itEnd(
		//		primitive.attributes.end());

		//	for (; it != itEnd; it++) {
		//		if ((it->first.compare("POSITION") == 0) ||
		//			(it->first.compare("NORMAL") == 0) ||
		//			(it->first.compare("TEXCOORD_0") == 0)) {
		//			if (content.m_GLProgramState.attribs[it->first] >= 0) {
		//				glDisableVertexAttribArray(content.m_GLProgramState.attribs[it->first]);
		//			}
		//		}
		//	}
		//}
	}
}

void printNodeVertices(Content& content)
{
	tinygltf::Model& model = content.m_model;
	for (auto& scene : model.scenes)
	{
		for (int node_index = 0; node_index < scene.nodes.size(); node_index++)
		{
			tinygltf::Node& node = model.nodes[scene.nodes[node_index]];
			std::cout << "NODE " << node.name << ":\n";

		}
	}
}
void printAllMeshData(Content& content)
{
	for (auto& mesh : content.m_model.meshes)
	{
		get_mesh_data(content, mesh);
	}
}