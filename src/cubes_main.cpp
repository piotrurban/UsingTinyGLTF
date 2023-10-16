#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <filesystem>

#include "gl_includes.h"

#include "content_drawing.h"


GLFWwindow* window;

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	
	if (!glfwInit())
	{
		printf("glfwInit failed. Exiting\n");
		exit(1);
	}
	window = glfwCreateWindow(500, 300, "Future cube window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	const std::filesystem::path model_path{"../model"};
	const std::filesystem::path model_gltf{ model_path / "triangle.gltf" };
	const std::filesystem::path model_vert{ model_path / "shader.vert" };
	const std::filesystem::path model_frag{ model_path / "shader.frag" };
	Content cubes_content(model_gltf.string(), model_vert.string(), model_frag.string());
	tinygltf::Model& cubes_model = cubes_content.m_model;
	tinygltf::Mesh& cubes_mesh = cubes_model.meshes[0];
	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		draw_mesh(cubes_content, cubes_mesh);
		glfwPollEvents();
	}
	
}