#include <iostream>

#include <gl_includes.h>

#include <BallTracker.h>
#include <Content.h>
#include <ContentUtils.h>
#include <TransformUtils.h>
#include "SimpleContent/SimpleContentFactory.h"
#include <content_drawing.h>
#include <GLMUtils.h>
#include <utils.h>
#include <format>

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <generated_model/models.h>

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
	constexpr int window_width{ 1800 };
	constexpr int window_height{ 1800 };
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
	//glfwSetMouseButtonCallback(window, onMouseClickRayCast);

	SimpleContentBuilder builder{};
	builder.setType(SimpleContentType::SQUARE);
	builder.setVertexShaderPath(pathToModels / "SimpleContentShaders/raymarching_base.vert");
	builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "raymarching.frag");
	builder.setUniforms({ "u_maxCount" });
	builder.addAnyUniform("u_InverseMVP");

	SimpleContent distance_content = builder.build();

	glViewport(0, 0, window_width, window_height);
	CheckErrors("viewport");

	//const glm::vec3 v0 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(0),1));
	//const glm::vec3 v1 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(1),1));
	//const glm::vec3 v2 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(2),1));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	float maxRaymarchingSteps{ 200.0F };

	const glm::mat4 persp = glm::perspective(45.0f * 3.1415926F / 180.0F, (float)window_width / (float)window_height, 0.1f, 100.0f);
	while (!glfwWindowShouldClose(window))
	{
		const glm::mat4 proj = ballTracker.getProjectionMat();
		const glm::mat4 camera = ballTracker.getModelMat();
		distance_content.setMV(glm::dmat4(camera));
		distance_content.setUniform("u_InverseMVP", glm::inverse(camera));
		distance_content.setUniform("u_maxCount", maxRaymarchingSteps);
		//if (ballTracker.m_updated)
		//{
		//	ballTracker.m_updated = false;
		//	std::cout << std::format("camera matrix = {}", camera) << std::endl;
		//}
		const float rayCos{ ballTracker.getCurrQuat()[2] };
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
			{
				maxRaymarchingSteps+=0.1;
				std::cout << "max raymarching steps = " << maxRaymarchingSteps << std::endl;
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
			{
				maxRaymarchingSteps-=0.1;
				std::cout << "max raymarching steps = " << maxRaymarchingSteps << std::endl;
			}
		}
		glfwPollEvents();

		glClearColor(0.0F, 0.0F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckErrors("clear");
		glEnable(GL_DEPTH_TEST);

		distance_content.draw();

		glFlush();
		glfwSwapBuffers(window);
	}
	distance_content.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();
}
