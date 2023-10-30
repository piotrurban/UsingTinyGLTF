#include <gl_includes.h>
#include <content_drawing.h>
#include <BallTracker.h>
#include "Handlers.h"
#include "NodeVisitors.h"

#include <utils.h>
#include <TransformUtils.h>
#include <ContentUtils.h>
#include <get_mesh_data.h>

#include <filesystem>
#include <iostream>
#include <chrono>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>

using namespace std::chrono_literals;
using namespace std::placeholders;

const float PI{ 3.1415926F };
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	if (!glfwInit())
	{
		std::cout << "glfwInit failed!\n";
		exit(1);
	}

	GLFWwindow* window;
	constexpr int window_width{ 800 };
	constexpr int window_height{ 800 };
	window = glfwCreateWindow(window_width, window_height, "scene graph example", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "glewInit failed!\n";
		exit(2);
	}

	BallTracker& ballTracker = BallTracker::getInstance();
	ballTracker.init();
	ballTracker.setWindowSize(window_width, window_height);
	ballTracker.registerWithGLFW(window);

	//glfwSetMouseButtonCallback(window, onMouseClickCbk);
	glfwSetMouseButtonCallback(window, onMouseClickRayCast);

	std::filesystem::path base_dir{ TOSTRING(PROJECT_INCLUDE_DIR) };
	const std::filesystem::path model_path{ base_dir / ".." / "model/scene_graph_example" };
	const std::filesystem::path model_gltf{ model_path / "scene_graph.gltf" };
	const std::filesystem::path model_vert{ model_path / "../shader_modern.vert" };
	const std::filesystem::path model_frag{ model_path / "../shader.frag" };
	Content scene_graph_content(model_gltf.string(), model_vert.string(), model_frag.string());
	scene_graph_content.setup_mesh_data();
	setCurrentContent(scene_graph_content);

	reshapeFunc(window, window_width, window_height);

	std::chrono::steady_clock::time_point time{ std::chrono::steady_clock::now() };

	const glm::mat4 persp = glm::perspective(45.0f * PI/180.0F, (float)window_width / (float)window_height, 0.1f, 100.0f);
	//scene_graph_content.m_perspectiveMat = persp;

	const auto corner = glm::vec4(1.0, 1.0, 1.0, 1.0);
	
	
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glMatrixMode(GL_PROJECTION);
	
		glPushMatrix();
		ballTracker.setCamera();
		const glm::mat4 proj = ballTracker.getProjectionMat();
		const glm::mat4 camera = ballTracker.getModelMat();
		scene_graph_content.m_perspectiveMat = glm::dmat4(persp*proj);
		scene_graph_content.m_viewMat =  camera;
		scene_graph_content.m_modelMat = glm::mat4(1.0);

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);


		if (std::chrono::steady_clock::now() - time > 2000ms)
		{
			//dumpMVPNodeVertices(scene_graph_content, 0);
			const auto pp_corner = persp * proj * corner;
			time = std::chrono::steady_clock::now();
			//printAllMeshData(scene_graph_content);
			//traverseModelMeshes(scene_graph_content);
		}
		draw_model(scene_graph_content);
		
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glFlush();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}