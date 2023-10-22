#include "Content.h"
#include "utils.h"

#include <iostream>

/// <summary>
/// @brief class for storing content of a deserialized glTF m_model with shaders
/// </summary>
Content::Content(const std::string& gltf_file, const std::string& vertex_shader_file, const std::string& fragment_shader_file)
	:
	m_model{}
	, m_vertId{}
	, m_fragId{}
	, m_progId{}
{
	load_from_file(gltf_file);
	load_and_link_shaders_from_files(vertex_shader_file, fragment_shader_file);
}

bool Content::load_and_link_shaders_from_files(const std::string& vertex_shader, const std::string& fragment_shader)
{
	if (false == LoadShader(GL_VERTEX_SHADER, m_vertId, vertex_shader.c_str())) {
		return false;
	}
	CheckErrors("load vert shader");

	if (false == LoadShader(GL_FRAGMENT_SHADER, m_fragId, fragment_shader.c_str())) {
		return false;
	}
	CheckErrors("load frag shader");

	if (false == LinkShader(m_progId, m_vertId, m_fragId)) {
		return false;
	}

	return true;
}

bool Content::load_from_file(const std::string& filename)
{
	tinygltf::TinyGLTF loader;
	std::string err, warn;
	const bool res = loader.LoadASCIIFromFile(&m_model, &err, &warn, filename);
	return res;
}

void Content::setup_mesh_data()
{

	for (size_t i = 0; i < m_model.bufferViews.size(); i++) {
		const tinygltf::BufferView& bufferView = m_model.bufferViews[i];
		if (bufferView.target == 0) {
			std::cout << "WARN: bufferView.target is zero" << std::endl;
			continue;  // Unsupported bufferView.
		}

		int sparse_accessor = -1;
		for (size_t a_i = 0; a_i < m_model.accessors.size(); ++a_i) {
			const auto& accessor = m_model.accessors[a_i];
			if (accessor.bufferView == i) {
				std::cout << i << " is used by accessor " << a_i << std::endl;
				if (accessor.sparse.isSparse) {
					std::cout
						<< "WARN: this bufferView has at least one sparse accessor to "
						"it. We are going to load the data as patched by this "
						"sparse accessor, not the original data"
						<< std::endl;
					sparse_accessor = a_i;
					break;
				}
			}
		}

		const tinygltf::Buffer& buffer = m_model.buffers[bufferView.buffer];
		GLBufferState state;
		glGenBuffers(1, &state.vb);
		glBindBuffer(bufferView.target, state.vb);
		std::cout << "buffer.size= " << buffer.data.size()
			<< ", byteOffset = " << bufferView.byteOffset << std::endl;

		if (sparse_accessor < 0)
			glBufferData(bufferView.target, bufferView.byteLength,
				&buffer.data.at(0) + bufferView.byteOffset,
				GL_STATIC_DRAW);
		else {
			const auto accessor = m_model.accessors[sparse_accessor];
			// copy the buffer to a temporary one for sparse patching
			unsigned char* tmp_buffer = new unsigned char[bufferView.byteLength];
			memcpy(tmp_buffer, buffer.data.data() + bufferView.byteOffset,
				bufferView.byteLength);

			const size_t size_of_object_in_buffer =
				ComponentTypeByteSize(accessor.componentType);
			const size_t size_of_sparse_indices =
				ComponentTypeByteSize(accessor.sparse.indices.componentType);

			const auto& indices_buffer_view =
				m_model.bufferViews[accessor.sparse.indices.bufferView];
			const auto& indices_buffer = m_model.buffers[indices_buffer_view.buffer];

			const auto& values_buffer_view =
				m_model.bufferViews[accessor.sparse.values.bufferView];
			const auto& values_buffer = m_model.buffers[values_buffer_view.buffer];

			for (size_t sparse_index = 0; sparse_index < accessor.sparse.count;
				++sparse_index) {
				int index = 0;
				// std::cout << "accessor.sparse.indices.componentType = " <<
				// accessor.sparse.indices.componentType << std::endl;
				switch (accessor.sparse.indices.componentType) {
				case TINYGLTF_COMPONENT_TYPE_BYTE:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
					index = (int)*(
						unsigned char*)(indices_buffer.data.data() +
							indices_buffer_view.byteOffset +
							accessor.sparse.indices.byteOffset +
							(sparse_index * size_of_sparse_indices));
					break;
				case TINYGLTF_COMPONENT_TYPE_SHORT:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
					index = (int)*(
						unsigned short*)(indices_buffer.data.data() +
							indices_buffer_view.byteOffset +
							accessor.sparse.indices.byteOffset +
							(sparse_index * size_of_sparse_indices));
					break;
				case TINYGLTF_COMPONENT_TYPE_INT:
				case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
					index = (int)*(
						unsigned int*)(indices_buffer.data.data() +
							indices_buffer_view.byteOffset +
							accessor.sparse.indices.byteOffset +
							(sparse_index * size_of_sparse_indices));
					break;
				}
				std::cout << "updating sparse data at index  : " << index
					<< std::endl;
				// index is now the target of the sparse index to patch in
				const unsigned char* read_from =
					values_buffer.data.data() +
					(values_buffer_view.byteOffset +
						accessor.sparse.values.byteOffset) +
					(sparse_index * (size_of_object_in_buffer * accessor.type));

				/*
				std::cout << ((float*)read_from)[0] << "\n";
				std::cout << ((float*)read_from)[1] << "\n";
				std::cout << ((float*)read_from)[2] << "\n";
				*/

				unsigned char* write_to =
					tmp_buffer + index * (size_of_object_in_buffer * accessor.type);

				memcpy(write_to, read_from, size_of_object_in_buffer * accessor.type);
			}

			// debug:
			/*for(size_t p = 0; p < bufferView.byteLength/sizeof(float); p++)
			{
			  float* b = (float*)tmp_buffer;
			  std::cout << "modified_buffer [" << p << "] = " << b[p] << '\n';
			}*/

			glBufferData(bufferView.target, bufferView.byteLength, tmp_buffer,
				GL_STATIC_DRAW);
			delete[] tmp_buffer;
		}
		glBindBuffer(bufferView.target, 0);

		m_bufferState[i] = state;
	}


#if 0  // TODO(syoyo): Implement
	// Texture
	{
		for (size_t i = 0; i < m_model.meshes.size(); i++) {
			const tinygltf::Mesh& mesh = m_model.meshes[i];

			gMeshState[mesh.name].diffuseTex.resize(mesh.primitives.size());
			for (size_t primId = 0; primId < mesh.primitives.size(); primId++) {
				const tinygltf::Primitive& primitive = mesh.primitives[primId];

				gMeshState[mesh.name].diffuseTex[primId] = 0;

				if (primitive.material < 0) {
					continue;
				}
				tinygltf::Material& mat = m_model.materials[primitive.material];
				// printf("material.name = %s\n", mat.name.c_str());
				if (mat.values.find("diffuse") != mat.values.end()) {
					std::string diffuseTexName = mat.values["diffuse"].string_value;
					if (m_model.textures.find(diffuseTexName) != m_model.textures.end()) {
						tinygltf::Texture& tex = m_model.textures[diffuseTexName];
						if (scene.images.find(tex.source) != m_model.images.end()) {
							tinygltf::Image& image = m_model.images[tex.source];
							GLuint texId;
							glGenTextures(1, &texId);
							glBindTexture(tex.target, texId);
							glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
							glTexParameterf(tex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameterf(tex.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

							// Ignore Texture.fomat.
							GLenum format = GL_RGBA;
							if (image.component == 3) {
								format = GL_RGB;
							}
							glTexImage2D(tex.target, 0, tex.internalFormat, image.width,
								image.height, 0, format, tex.type,
								&image.image.at(0));

							CheckErrors("texImage2D");
							glBindTexture(tex.target, 0);

							printf("TexId = %d\n", texId);
							gMeshState[mesh.name].diffuseTex[primId] = texId;
						}
					}
				}
			}
		}
}
#endif

	glUseProgram(m_progId);
	GLint vtloc = glGetAttribLocation(m_progId, "in_vertex");
	GLint nrmloc = glGetAttribLocation(m_progId, "in_normal");
	GLint uvloc = glGetAttribLocation(m_progId, "in_texcoord");

	// GLint diffuseTexLoc = glGetUniformLocation(progId, "diffuseTex");
	GLint isCurvesLoc = glGetUniformLocation(m_progId, "uIsCurves");

	m_GLProgramState.attribs["POSITION"] = vtloc;
	m_GLProgramState.attribs["NORMAL"] = nrmloc;
	m_GLProgramState.attribs["TEXCOORD_0"] = uvloc;
	// m_GLProgramState.uniforms["diffuseTex"] = diffuseTexLoc;
	m_GLProgramState.uniforms["isCurvesLoc"] = isCurvesLoc;


}