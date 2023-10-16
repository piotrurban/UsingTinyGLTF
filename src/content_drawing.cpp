#include "content_drawing.h"

void draw_mesh(tinygltf::Model& model, const tinygltf::Mesh& mesh)
{
	//// Skip curves primitive.
 // if (gCurvesMesh.find(mesh.name) != gCurvesMesh.end()) {
 //  return;
 //}

 // if (gGLProgramState.uniforms["diffuseTex"] >= 0) {
 //  glUniform1i(gGLProgramState.uniforms["diffuseTex"], 0);  // TEXTURE0
 //}

	if (gGLProgramState.uniforms["isCurvesLoc"] >= 0) {
		glUniform1i(gGLProgramState.uniforms["isCurvesLoc"], 0);
	}

	for (size_t i = 0; i < mesh.primitives.size(); i++) {
		const tinygltf::Primitive& primitive = mesh.primitives[i];

		if (primitive.indices < 0) return;

		// Assume TEXTURE_2D target for the texture object.
		// glBindTexture(GL_TEXTURE_2D, gMeshState[mesh.name].diffuseTex[i]);

		std::map<std::string, int>::const_iterator it(primitive.attributes.begin());
		std::map<std::string, int>::const_iterator itEnd(
			primitive.attributes.end());

		for (; it != itEnd; it++) {
			assert(it->second >= 0);
			const tinygltf::Accessor& accessor = model.accessors[it->second];
			glBindBuffer(GL_ARRAY_BUFFER, gBufferState[accessor.bufferView].vb);
			CheckErrors("bind buffer");
			int size = 1;
			if (accessor.type == TINYGLTF_TYPE_SCALAR) {
				size = 1;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC2) {
				size = 2;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC3) {
				size = 3;
			}
			else if (accessor.type == TINYGLTF_TYPE_VEC4) {
				size = 4;
			}
			else {
				assert(0);
			}
			// it->first would be "POSITION", "NORMAL", "TEXCOORD_0", ...
			if ((it->first.compare("POSITION") == 0) ||
				(it->first.compare("NORMAL") == 0) ||
				(it->first.compare("TEXCOORD_0") == 0)) {
				if (gGLProgramState.attribs[it->first] >= 0) {
					// Compute byteStride from Accessor + BufferView combination.
					int byteStride =
						accessor.ByteStride(model.bufferViews[accessor.bufferView]);
					assert(byteStride != -1);
					glVertexAttribPointer(gGLProgramState.attribs[it->first], size,
						accessor.componentType,
						accessor.normalized ? GL_TRUE : GL_FALSE,
						byteStride, BUFFER_OFFSET(accessor.byteOffset));
					CheckErrors("vertex attrib pointer");
					glEnableVertexAttribArray(gGLProgramState.attribs[it->first]);
					CheckErrors("enable vertex attrib array");
				}
			}
		}

		const tinygltf::Accessor& indexAccessor =
			model.accessors[primitive.indices];
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			gBufferState[indexAccessor.bufferView].vb);
		CheckErrors("bind buffer");
		int mode = -1;
		if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
			mode = GL_TRIANGLES;
		}
		else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
			mode = GL_TRIANGLE_STRIP;
		}
		else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
			mode = GL_TRIANGLE_FAN;
		}
		else if (primitive.mode == TINYGLTF_MODE_POINTS) {
			mode = GL_POINTS;
		}
		else if (primitive.mode == TINYGLTF_MODE_LINE) {
			mode = GL_LINES;
		}
		else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
			mode = GL_LINE_LOOP;
		}
		else {
			assert(0);
		}
		glDrawElements(mode, indexAccessor.count, indexAccessor.componentType,
			BUFFER_OFFSET(indexAccessor.byteOffset));
		CheckErrors("draw elements");

		{
			std::map<std::string, int>::const_iterator it(
				primitive.attributes.begin());
			std::map<std::string, int>::const_iterator itEnd(
				primitive.attributes.end());

			for (; it != itEnd; it++) {
				if ((it->first.compare("POSITION") == 0) ||
					(it->first.compare("NORMAL") == 0) ||
					(it->first.compare("TEXCOORD_0") == 0)) {
					if (gGLProgramState.attribs[it->first] >= 0) {
						glDisableVertexAttribArray(gGLProgramState.attribs[it->first]);
					}
				}
			}
		}
	}
}
