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

#include "Content.h"
#include "TinyGltfTools/TransformUtils.h"
#include "TinyGltfTools/BallTracker.h"
#include "TinyGltfTools/trackball.h"

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

	if (glewInit() != GLEW_OK)
	{
		printf("glewInit failed, exiting\n");
		exit(2);
	}
	BallTracker& ballTracker = BallTracker::getInstance();
	ballTracker.init();
	ballTracker.setWindowSize(768.0, 768.0);
	ballTracker.registerWithGLFW(window);

	std::filesystem::path base_dir{ __FILE__"\\..\\.."};
	const std::filesystem::path model_path{base_dir/".."/"model"};
	const std::filesystem::path model_gltf{ model_path / "triangle.gltf" };
	const std::filesystem::path model_vert{ model_path / "shader.vert" };
	const std::filesystem::path model_frag{ model_path / "shader.frag" };
	Content cubes_content(model_gltf.string(), model_vert.string(), model_frag.string());
	tinygltf::Model& cubes_model = cubes_content.m_model;
	tinygltf::Mesh& cubes_mesh = cubes_model.meshes[0];
	cubes_content.setup_mesh_data();
	reshapeFunc(window, 768, 768);
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		ballTracker.setCamera();

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		draw_mesh(cubes_content, cubes_mesh);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glFlush();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}