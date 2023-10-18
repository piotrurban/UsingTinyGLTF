#include "TransformUtils.h"
#include "gl_includes.h"

void reshapeFunc(GLFWwindow* window, int w, int h) {
	(void)window;
	int fb_w, fb_h;
	glfwGetFramebufferSize(window, &fb_w, &fb_h);
	glViewport(0, 0, fb_w, fb_h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}