#include "BallTracker.h"
#include "TinyGltfTools/trackball.h"

std::unique_ptr<BallTracker> BallTracker::s_instance{ nullptr };
float BallTracker::s_camZ{ 3.0F };


void BallTracker::init()
{
	trackball(curr_quat, 0, 0, 0, 0);

	eye[0] = 0.0f;
	eye[1] = 0.0f;
	eye[2] = s_camZ;

	lookat[0] = 0.0f;
	lookat[1] = 0.0f;
	lookat[2] = 0.0f;

	up[0] = 0.0f;
	up[1] = 1.0f;
	up[2] = 0.0f;
}

void BallTracker::setWindowSize(float _width, float _height)
{
	width = _width;
	height = _height;
}

void BallTracker::registerWithGLFW(GLFWwindow* window)
{
	glfwSetCursorPosCallback(window, BallTracker::motionFunc);
}

BallTracker&  BallTracker::getInstance()
{
	if (!s_instance)
	{
		s_instance.reset(new BallTracker());
	}
	return *s_instance;
}

void BallTracker::motionFunc(GLFWwindow* window, double mouse_x, double mouse_y)
{
	BallTracker::getInstance().implMotionFunc(window, mouse_x, mouse_y);
}

inline float(&BallTracker::getCurrQuat())[4]
{
	return curr_quat;
}

void BallTracker::setCamera()
{
	GLfloat mat[4][4];
	build_rotmatrix(mat, curr_quat);
	glMatrixMode(GL_PROJECTION);
	gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0],
		up[1], up[2]);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&mat[0][0]);
}

void BallTracker::implMotionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
	(void)window;
	float rotScale = 1.0f;
	float transScale = 2.0f;
	const bool mouseLeftPressed{ glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS };
	const bool mouseMiddlePressed{ glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS };
	const bool mouseRightPressed{ glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS };

	if (mouseLeftPressed) {
		trackball(prev_quat, rotScale * (2.0f * prevMouseX - width) / (float)width,
			rotScale * (height - 2.0f * prevMouseY) / (float)height,
			rotScale * (2.0f * mouse_x - width) / (float)width,
			rotScale * (height - 2.0f * mouse_y) / (float)height);

		add_quats(prev_quat, curr_quat, curr_quat);
	}
	else if (mouseMiddlePressed) {
		eye[0] += -transScale * (mouse_x - prevMouseX) / (float)width;
		lookat[0] += -transScale * (mouse_x - prevMouseX) / (float)width;
		eye[1] += transScale * (mouse_y - prevMouseY) / (float)height;
		lookat[1] += transScale * (mouse_y - prevMouseY) / (float)height;
	}
	else if (mouseRightPressed) {
		eye[2] += transScale * (mouse_y - prevMouseY) / (float)height;
		lookat[2] += transScale * (mouse_y - prevMouseY) / (float)height;
	}

	// Update mouse point
	prevMouseX = mouse_x;
	prevMouseY = mouse_y;
}