#version 450

in vec3    in_vertex;
in vec3    in_normal;
in vec2    in_texcoord;

uniform mat4 u_MVP;
//uniform mat4 u_MVInvTrans;

out vec3      local_normal;

void main(void)
{
	vec4 p = u_MVP * vec4(in_vertex, 1);
	gl_Position = p;
	local_normal = in_normal;

}
