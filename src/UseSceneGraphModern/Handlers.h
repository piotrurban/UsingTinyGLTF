#pragma once
#include <gl_includes.h>
#include <Content.h>

void setCurrentContent(const Content& content);

void onMouseClickRayCast(GLFWwindow* window, int button, int action, int mods);
