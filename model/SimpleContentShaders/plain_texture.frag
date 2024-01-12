#version 450

in vec2 coord;
out vec4 color;

uniform sampler2D u_textureSampler;

void main()
{
	color = texture(u_textureSampler, vec2(coord.x, -coord.y));
	color.w = 1.0;
}