#pragma once
#include <memory>
#include "gl_includes.h"

class BallTracker
{
private:
	BallTracker() = default;

public:
	~BallTracker() = default;
	void init();
	void setWindowSize(float width, float height);
	void registerWithGLFW(GLFWwindow* window);

	static BallTracker& getInstance();
	static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y);
	float(&getCurrQuat())[4];
	void setCamera();

private:
	void implMotionFunc(GLFWwindow* window, double mouse_x, double mouse_y);

private:
	static std::unique_ptr<BallTracker> s_instance;
	static float s_camZ;
	float curr_quat[4];
	float prev_quat[4];
	float eye[3], lookat[3], up[3];
	float prevMouseX{ 0.0F };
	float prevMouseY{ 0.0F };
	float width{ 768.0F };
	float height{ 768.0F };
};


