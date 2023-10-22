#include <gl_includes.h>
#include <content_drawing.h>
#include <BallTracker.h>
#include <utils.h>
#include <TransformUtils.h>

#include <filesystem>
#include <iostream>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

void printNodeVertices(Content& content)
{
	tinygltf::Model& model = content.m_model;
	for (auto& scene : model.scenes)
	{
		for (int node_index = 0; node_index < scene.nodes.size(); node_index++)
		{
			tinygltf::Node& node = model.nodes[scene.nodes[node_index]];
			std::cout << "NODE " << node.name << ":\n";
			
		}
	}
}
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
	constexpr int window_height{ 600 };
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

	std::filesystem::path base_dir{ TOSTRING(PROJECT_INCLUDE_DIR) };
	const std::filesystem::path model_path{ base_dir / ".." / "model/scene_graph_example" };
	const std::filesystem::path model_gltf{ model_path / "scene_graph.gltf" };
	const std::filesystem::path model_vert{ model_path / "../shader.vert" };
	const std::filesystem::path model_frag{ model_path / "../shader.frag" };
	Content scene_graph_content(model_gltf.string(), model_vert.string(), model_frag.string());
	scene_graph_content.setup_mesh_data();
	reshapeFunc(window, window_width, window_height);

	printNodeVertices(scene_graph_content);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		ballTracker.setCamera();

		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		draw_model(scene_graph_content);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glFlush();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}