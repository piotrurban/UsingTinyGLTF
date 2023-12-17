#pragma once
#include <gl_includes.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <array>
#include <iostream>
#include <format>

void reshapeFunc(GLFWwindow* window, int w, int h);

void triangleRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy);

void testTriangleRayCast();

std::pair<float, float> windowToNormCoord(int win_x, int win_y, GLFWwindow* window);

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec3);

glm::vec3 triangleZRayCast(const std::array<glm::vec3, 3>& triangle_verts, const glm::vec2& projection_xy);

void onMouseClickCbk(GLFWwindow* window, int button, int action, int mods);

template <>
struct std::formatter<glm::vec3> {
	char presentation = 'd'; // default presentation

	// Parses format specifications of the form ['d' | 'x'].
	constexpr auto parse(std::format_parse_context& ctx) {
		// [ctx.begin(), ctx.end()) is a character range that contains a part of the format string.
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && (*it == 'd' || *it == 'x')) {
			presentation = *it++;
		}
		// Check if reached the end of the range:
		if (it != end && *it != '}') {
			throw std::format_error("invalid format");
		}
		// Return an iterator past the end of the parsed range:
		return it;
	}

	// Formats the value as an integer in decimal ('d') or hexadecimal ('x') notation.
	template <typename FormatContext>
	auto format(glm::vec3 value, FormatContext& ctx) const -> decltype(ctx.out()) {
		return format_to(ctx.out(), "vec3({},{},{})", value.x, value.y, value.z);

	}
};

template<>
struct std::formatter<glm::mat4> {
	constexpr auto parse(std::format_parse_context& ctx) {
		return ctx.end();
	}

	template<class FormatContext>
	auto format(glm::mat4 value, FormatContext& ctx) const -> decltype(ctx.out()) {
		return format_to(ctx.out(), "mat4 ( {}, {}, {}, {} \n {}, {}, {}, {} \n {}, {}, {}, {} \n {}, {}, {}, {} )\n",
			value[0][0], value[0][1], value[0][2], value[0][3],
			value[1][0], value[1][1], value[1][2], value[1][3],
			value[2][0], value[2][1], value[2][2], value[2][3],
			value[3][0], value[3][1], value[3][2], value[3][3]);
	}
};

bool isTriangleInViewport(const std::array<glm::vec3, 3>& triangle);
