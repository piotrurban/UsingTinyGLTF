attribute vec3    in_vertex;

varying vec2 coord;

void main(void)
{
	vec4 p = vec4(in_vertex, 1);
	gl_Position = p;
	coord = in_vertex.xy;
}
