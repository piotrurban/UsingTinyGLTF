#include <iostream>

#include <gl_includes.h>

#include <BallTracker.h>
#include <Content.h>
#include <ContentUtils.h>
#include <TransformUtils.h>
#include "SimpleContent/SimpleContentFactory.h"
#include <content_drawing.h>
#include <GLMUtils.h>
#include <GLFWUtils.h>
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
	GLFWwindow* windowHUD;
	constexpr int window_width{ 1800 };
	constexpr int window_height{ 1800 };
	window = glfwCreateWindow(window_width, window_height, "scene graph example", nullptr, nullptr);
	windowHUD = glfwCreateWindow(window_width / 3, window_height / 3, "HUD window", nullptr, nullptr);
	registerResizeCallback(window);
	registerResizeCallback(windowHUD);
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
	builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "multiple_objects.frag");
	builder.setTexturePath(pathToModels / "cyclope.png");
	builder.setUniforms({ "u_maxCount", "u_cameraZ" });
	builder.addAnyUniform("u_InverseMVP");

	SimpleContent distance_content = builder.build();

	glfwMakeContextCurrent(windowHUD);
	builder.setFragmentShaderPath(pathToModels / "SimpleContentShaders" / "plain_texture.frag");
	SimpleContent hud_content = builder.build();

	glfwMakeContextCurrent(window);
	glViewport(0, 0, window_width, window_height);
	CheckErrors("viewport");
	
	glfwMakeContextCurrent(windowHUD);
	glViewport(0, 0, window_width/3, window_height/3);
	CheckErrors("viewport");

	//const glm::vec3 v0 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(0),1));
	//const glm::vec3 v1 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(1),1));
	//const glm::vec3 v2 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(2),1));
	glfwMakeContextCurrent(window);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glfwMakeContextCurrent(windowHUD);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float maxRaymarchingSteps{ 200.0F };
	float cameraZ{ 0.0F };

	const glm::mat4 persp = glm::perspective(45.0f * 3.1415926F / 180.0F, (float)window_width / (float)window_height, 0.1f, 100.0f);
	while (!glfwWindowShouldClose(window) || !glfwWindowShouldClose(windowHUD))
	{
		glfwMakeContextCurrent(window);
		const glm::mat4 proj = ballTracker.getProjectionMat();
		const glm::mat4 camera = ballTracker.getModelMat();
		distance_content.setMV(glm::dmat4(camera));
		distance_content.setUniform("u_InverseMVP", glm::inverse(camera));
		distance_content.setUniform("u_maxCount", maxRaymarchingSteps);
		distance_content.setUniform("u_cameraZ", cameraZ);
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
				maxRaymarchingSteps += 0.1;
				cameraZ += 0.1;
				std::cout << "cameraZ = " << cameraZ << std::endl;
			}
			else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
			{
				maxRaymarchingSteps -= 0.1;
				cameraZ -= 0.1;
				std::cout << "cameraZ = " << cameraZ << std::endl;
			}
		}

		glfwPollEvents();

		glClearColor(0.5F, 0.5F, 0.6F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckErrors("clear");
		glEnable(GL_DEPTH_TEST);

		distance_content.draw();
		glfwSwapBuffers(window);
		glFlush();

		glfwMakeContextCurrent(windowHUD);

		glfwPollEvents();

		glClearColor(0.0F, 0.0F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckErrors("clear");
		glEnable(GL_DEPTH_TEST);

		hud_content.draw();
		glfwSwapBuffers(windowHUD);
		glFlush();
	}
	distance_content.cleanup();
	hud_content.cleanup();
	glfwDestroyWindow(window);
	glfwDestroyWindow(windowHUD);
	glfwTerminate();
}
