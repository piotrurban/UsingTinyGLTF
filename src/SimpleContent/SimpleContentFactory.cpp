#include "SimpleContentFactory.h"
#include <filesystem>

namespace fs = std::filesystem;

SimpleContent getSimpleContent(SimpleContentType type)
{
	fs::path vertexShaderPath{ SimpleContent::s_defaultVertexShader };
	fs::path fragmentShaderPath{ SimpleContent::s_defaultFragmentShader };
	GLenum mode = GL_TRIANGLES;
	switch (type)
	{
	case SimpleContentType::DEFAULT:
	{
		const std::vector<glm::vec3> vertices{ {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F }, {-1.0F, 1.0F, 0.0F} };
		const std::vector<unsigned short> indices{ 0, 1, 3, 3, 1, 2 };
		return SimpleContent{ vertices, indices };
		break;
	}
	case SimpleContentType::CIRCLE:
	{
		const std::vector<glm::vec3> vertices{ {-1.0F, -1.0F, 0.0F}, {1.0F, -1.0F, 0.0F}, {1.0F, 1.0F, 0.0F }, {-1.0F, 1.0F, 0.0F} };
		const std::vector<unsigned short> indices{ 0, 1, 3, 3, 1, 2 };
		SimpleContent sc{ vertices, indices, GL_TRIANGLES, SimpleContent::s_defaultVertexShader,
			SimpleContent::s_circleFragmentShader, {"u_radius"} };
		sc.setMV(glm::mat4(1.0F));
		sc.setUniform("u_radius", 0.25F);
		return sc;
		break;
	}
	}

}
