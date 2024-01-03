#include "shader_control.h"
using namespace ImGui;

ShaderController::ShaderData ShaderController::shaderData{};

void ShaderController::displayControls()
{
	PushItemWidth(GetFontSize() * 20);
	ColorEdit3("diffuse color", shaderData.diffuse);
	ColorEdit3("specular color", shaderData.specular);
	PopItemWidth();
}
