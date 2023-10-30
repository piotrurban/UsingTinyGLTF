#pragma once
#include <gl_includes.h>
#include <Content.h>
#include <SimpleContent/SimpleContent.h>

void setCurrentContent(const Content& content);

void setCurrentMousePointerIcon(SimpleContent& content);

void onMouseClickRayCast(GLFWwindow* window, int button, int action, int mods);

void onMouseClickMoveCircle(GLFWwindow* window, int button, int action, int mods);
