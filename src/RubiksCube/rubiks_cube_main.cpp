#include <iostream>

#include <gl_includes.h>

#include <BallTracker.h>
#include <Content.h>
#include <ContentUtils.h>
#include <content_drawing.h>
#include <GLMUtils.h>
#include <utils.h>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <generated_model/models.h>

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	if (!glfwInit())
	{
		std::cout << "glfwInit failed!\n";
		_sleep(5000);
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
		_sleep(5000);
		exit(2);
	}

	BallTracker& ballTracker = BallTracker::getInstance();
	ballTracker.init();
	ballTracker.setWindowSize(window_width, window_height);
	ballTracker.registerWithGLFW(window);

	//glfwSetMouseButtonCallback(window, onMouseClickCbk);
	//glfwSetMouseButtonCallback(window, onMouseClickRayCast);

	const std::filesystem::path model_path{ pathToModels };
	const std::filesystem::path model_gltf{ model_path / "textured_cube/textured_cube.gltf" };
	const std::filesystem::path model_vert{ model_path / "shader_rubik.vert" };
	const std::filesystem::path model_frag{ model_path / "shader_rubik.frag" };
	Content rubiks_cube_content(model_gltf.string(), model_vert.string(), model_frag.string());
	rubiks_cube_content.setup_mesh_data();
	std::map<std::tuple<int, int, int>, unsigned short> coords_to_node_id{};
	int currentId{ 1 };

	for (int x = -1; x < 2; x++)
		for (int y = -1; y < 2; y++)
			for (int z = -1; z < 2; z++)
			{
				coords_to_node_id[std::make_tuple(x, y, z)] = currentId;
				auto new_node = rubiks_cube_content.m_model.nodes.at(0);
				auto new_mesh = rubiks_cube_content.m_model.meshes.at(0);
				const int new_mesh_id = rubiks_cube_content.m_model.meshes.size();
				const int new_node_id = rubiks_cube_content.m_model.nodes.size();
				new_node.mesh = new_mesh_id;
				new_node.translation = std::vector<double>{ x * 2.5, y * 2.5, z * 2.5 };
				rubiks_cube_content.m_model.meshes.push_back(new_mesh);
				rubiks_cube_content.m_model.nodes.push_back(new_node);
				rubiks_cube_content.m_model.scenes.at(0).nodes.push_back(new_node_id);
				currentId++;
			}

	const glm::mat4 persp = glm::perspective(45.0f * 3.1415926F / 180.0F, (float)window_width / (float)window_height, 0.1f, 100.0f);

	glEnable(GL_BLEND);
	CheckErrors("blend");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	CheckErrors("alpha");

	float angle = 0.0F;
	float delta = 0.05F;
	while (!glfwWindowShouldClose(window))
	{
		_sleep(20);
		angle += delta;
		glm::quat rotZQuat = glm::angleAxis(glm::radians(delta), glm::vec3(0.0F, 0.0F, 1.0F));
		std::vector<double> rotZQuatAsVector{ rotZQuat.x, rotZQuat.y, rotZQuat.z, rotZQuat.w };
		for (auto& coordNode : coords_to_node_id)
		{
			const auto& [x, y, z] = coordNode.first;
			if (z == -1)
			{
				glm::vec3 transl = glm::make_vec3(rubiks_cube_content.m_model.nodes[coordNode.second].translation.data());
				transl = rotZQuat * transl;
				rubiks_cube_content.m_model.nodes[coordNode.second].translation = { transl.x, transl.y, transl.z };
				//glm::vec4 rotat = glm::make_vec4(rubiks_cube_content.m_model.nodes[coordNode.second].rotation.data());
				glm::quat rotatQ = glm::angleAxis(glm::radians(angle), glm::vec3(0.0F, 0.0F, 1.0F));
				rubiks_cube_content.m_model.nodes[coordNode.second].rotation = { rotatQ.x, rotatQ.y, rotatQ.z, rotatQ.w };
			}
		}
		//ballTracker.setCamera();
		const glm::mat4 proj = ballTracker.getProjectionMat();
		const glm::mat4 camera = ballTracker.getModelMat();
		rubiks_cube_content.m_perspectiveMat = glm::dmat4(persp * proj);
		rubiks_cube_content.m_viewMat = camera;
		rubiks_cube_content.m_modelMat = glm::mat4(1.0);
		glfwPollEvents();

		glClearColor(0.0F, 0.0F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckErrors("clear");
		glEnable(GL_DEPTH_TEST);

		draw_model(rubiks_cube_content);

		glFlush();
		glfwSwapBuffers(window);
	}
}