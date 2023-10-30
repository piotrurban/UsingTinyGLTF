#include "SimpleContent.h"
#include "utils.h"
#include "GLMUtils.h"
#include <iostream>

int main()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_VERSION_MINOR, 5);

	if (!glfwInit())
	{
		std::cerr << "glfwInit failed. Exiting\n";
		exit(-1);
	}
	GLFWwindow* window;
	int windowWidth = 800;
	int windowHeight = 800;
	window = glfwCreateWindow(windowWidth, windowHeight, "simple content", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "glewInit failed. Exiting\n";
		exit(-1);
	}

	const std::vector<glm::vec3> triangleVerts = { {0.0F, 0.0F, 0.0F}, {1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F} };
	const std::vector<unsigned short> indices = { 0,1,2 };
	SimpleContent sc{ triangleVerts, indices, GL_TRIANGLES, SimpleContent::s_defaultVertexShader, SimpleContent::s_circleFragmentShader };
	const glm::mat4 perspective = glm::perspective(45.0F * 3.1415926F / 180.0F, (float)windowWidth / (float)windowHeight, 0.1F, 100.0F);
	const glm::mat4 worldview = glm::lookAt(glm::vec3(0.0F, 0.0F, 3.0F), glm::vec3(0.0F, 0.0F, 0.0F), glm::vec3(0.0F, 1.0F, 0.0F));
	sc.setMV(perspective * worldview);
	
	const std::vector<glm::vec3> flatTriangle = { { -0.25F, -0.25F, 0.0F}, {0.25F, -0.25F, 0.0F}, {0.0F, 0.3F, 0.0F} };
	const std::vector<unsigned short> flatIndices = { 0,1,2 };
	SimpleContent scflat{ flatTriangle, flatIndices, GL_LINE_LOOP };
	scflat.setMV(glm::mat4(1.0F));

	glViewport(0, 0, windowWidth, windowHeight);
	CheckErrors("viewport");

	//const glm::vec3 v0 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(0),1));
	//const glm::vec3 v1 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(1),1));
	//const glm::vec3 v2 = perspectiveDivide3(sc.m_mv * glm::vec4(triangleVerts.at(2),1));
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float shift = 0.0F;
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.0F, 0.0F, 0.3F, 1.0F);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		CheckErrors("clear");
		glEnable(GL_DEPTH_TEST);

		shift += 0.0001F;
		sc.setMV(perspective * worldview * glm::translate(glm::mat4(1.0), glm::vec3(shift, shift, 0.0F)));
		sc.draw();
		scflat.draw();

		glFlush();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
}