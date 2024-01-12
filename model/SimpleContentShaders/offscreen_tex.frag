#version 450

in vec2 coord;
out vec4 color;

uniform sampler2D u_textureSampler;
uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform float u_sel_tex;
uniform uint u_render_mode;

void main()
{
	vec2 coord = 0.5 + 0.5*vec2(coord.x, coord.y);
	vec4 c1 = texture(u_tex1, vec2(coord.x, coord.y));
	vec4 c2 = texture(u_tex2, vec2(coord.x, coord.y));
	color = mix(c1, c2, u_sel_tex);
	color.w = 1.0;

	if (u_render_mode == 0)
	{
		// from tex1 to tex2
		color = texture(u_tex1, vec2(coord.x - 0.1, coord.y));
		//color = vec4(0.0,1.0,0.0,1.0);
	}
	else if (u_render_mode == 1)
	{
		// from tex2 to fb
		color = texture(u_tex2, vec2(coord.x, coord.y));
		//color = vec4(1.0,1.0,0.0,1.0);
	}
	else if (u_render_mode == 2)
	{
		// from tex2 to tex1
		color = texture(u_tex2, vec2(coord.x - 0.1, coord.y));
	}
	else if (u_render_mode == 3)
	{
		// from tex1 to fb
		color = texture(u_tex1, vec2(coord.x, coord.y));
		//color = vec4(0.0,0.0,1.0,1.0);
	}
	else
	{
		color = vec4(1.0,0.0,0.0,1.0);
	}
	color.w = 1.0;
}