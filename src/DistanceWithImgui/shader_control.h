#include "imgui.h"
#include <SimpleContent/SimpleContent.h>
namespace ShaderController
{
	struct ShaderData
	{
		float diffuse[3];
		float specular[3];
	};
	extern ShaderData shaderData;
	void displayControls();
	void displaySimpleContentControls(SimpleContent& content);
}