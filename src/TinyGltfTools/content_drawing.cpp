#include "content_drawing.h"
#include "utils.h"

void draw_model(Content& content) {
	tinygltf::Model& model = content.m_model;
#if 0
	std::map<std::string, tinygltf::Mesh>::const_iterator it(scene.meshes.begin());
	std::map<std::string, tinygltf::Mesh>::const_iterator itEnd(scene.meshes.end());

	for (; it != itEnd; it++) {
		DrawMesh(scene, it->second);
		DrawCurves(scene, it->second);
	}
#else
	// If the glTF asset has at least one scene, and doesn't define a default one
	// just show the first one we can find
	assert(model.scenes.size() > 0);
	int scene_to_display = model.defaultScene > -1 ? model.defaultScene : 0;
	const tinygltf::Scene& scene = model.scenes[scene_to_display];
	for (size_t i = 0; i < scene.nodes.size(); i++) {
		draw_node(content, model.nodes[scene.nodes[i]]);
	}
#endif
}
// Hierarchically draw nodes
void draw_node(Content& content, const tinygltf::Node& node) {
	// Apply xform

	tinygltf::Model& model = content.m_model;
	glPushMatrix();
	if (node.matrix.size() == 16) {
		// Use `matrix' attribute
		glMultMatrixd(node.matrix.data());
	}
	else {
		// Assume Trans x Rotate x Scale order
		if (node.translation.size() == 3) {
			glTranslated(node.translation[0], node.translation[1],
				node.translation[2]);
		}

		if (node.rotation.size() == 4) {
			double angleDegrees;
			double axis[3];

			QuatToAngleAxis(node.rotation, angleDegrees, axis);

			glRotated(angleDegrees, axis[0], axis[1], axis[2]);
		}

		if (node.scale.size() == 3) {
			glScaled(node.scale[0], node.scale[1], node.scale[2]);
		}
	}

	// std::cout << "node " << node.name << ", Meshes " << node.meshes.size() <<
	// std::endl;

	// std::cout << it->first << std::endl;
	// FIXME(syoyo): Refactor.
	// DrawCurves(scene, it->second);
	if (node.mesh > -1) {
		assert(node.mesh < model.meshes.size());
		draw_mesh(content, model.meshes[node.mesh]);
	}

	// Draw child nodes.
	for (size_t i = 0; i < node.children.size(); i++) {
		assert(node.children[i] < model.nodes.size());
		draw_node(content, model.nodes[node.children[i]]);
	}

	glPopMatrix();
}
void draw_mesh(Content& content, tinygltf::Mesh& mesh)
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

		for (; it != itEnd; it++) {
			assert(it->second >= 0);
			const tinygltf::Accessor& accessor = model.accessors[it->second];
			glBindBuffer(GL_ARRAY_BUFFER, content.m_bufferState[accessor.bufferView].vb);
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
				if (content.m_GLProgramState.attribs[it->first] >= 0) {
					// Compute byteStride from Accessor + BufferView combination.
					int byteStride =
						accessor.ByteStride(model.bufferViews[accessor.bufferView]);
					assert(byteStride != -1);
					glVertexAttribPointer(content.m_GLProgramState.attribs[it->first], size,
						accessor.componentType,
						accessor.normalized ? GL_TRUE : GL_FALSE,
						byteStride, BUFFER_OFFSET(accessor.byteOffset));
					CheckErrors("vertex attrib pointer");
					glEnableVertexAttribArray(content.m_GLProgramState.attribs[it->first]);
					CheckErrors("enable vertex attrib array");
				}
			}
		}

		const tinygltf::Accessor& indexAccessor =
			model.accessors[primitive.indices];
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
			content.m_bufferState[indexAccessor.bufferView].vb);
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
					if (content.m_GLProgramState.attribs[it->first] >= 0) {
						glDisableVertexAttribArray(content.m_GLProgramState.attribs[it->first]);
					}
				}
			}
		}
	}
}
