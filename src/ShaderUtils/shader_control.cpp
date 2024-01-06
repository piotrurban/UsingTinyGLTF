#include "shader_control.h"
#include <glm/gtc/type_ptr.hpp>
#include <any>

using namespace ImGui;

ShaderController::ShaderData ShaderController::shaderData{};

void ShaderController::displayControls()
{
	PushItemWidth(GetFontSize() * 20);
	ColorEdit3("diffuse color", shaderData.diffuse);
	ColorEdit3("specular color", shaderData.specular);
	PopItemWidth();
}

void ShaderController::displaySimpleContentControls(SimpleContent& content)
{
	for (auto& [name, type] : content.m_uniforms)
	{
		switch (type)
		{
		case GL_FLOAT:
		{
			float val = any_cast<float>(content.m_anyUniformMap.at(name));
			if (DragFloat(name.c_str(), &val))
			{
				content.m_anyUniformMap[name] = val;
			}
			break;
		}
		case GL_FLOAT_VEC3:
		{
			glm::vec3 val = any_cast<glm::vec3>(content.m_anyUniformMap.at(name));
			if (ColorEdit3(name.c_str(), glm::value_ptr(val)))
			{
				content.m_anyUniformMap[name] = val;
			}
			if (DragFloat3(name.c_str(), glm::value_ptr(val)))
			{
				content.m_anyUniformMap[name] = val;
			}
			break;
		}
		case GL_FLOAT_VEC4:
		{
			glm::vec4 val = any_cast<glm::vec4>(content.m_anyUniformMap.at(name));
			if (ColorEdit4(name.c_str(), glm::value_ptr(val)))
			{
				content.m_anyUniformMap[name] = val;
			}
			if (DragFloat4(name.c_str(), glm::value_ptr(val)))
			{
				content.m_anyUniformMap[name] = val;
			}
			break;
		}
		default:
			break;
		}

	}
}



