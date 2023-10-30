attribute vec3    in_vertex;
attribute vec3    in_normal;
attribute vec2    in_texcoord;

uniform mat4 u_MVP;
//uniform mat4 u_MVInvTrans;

varying vec3      normal;
varying vec2      texcoord;

out vec2 coord;

void main(void)
{
	//vec4 p = gl_ModelViewProjectionMatrix * vec4(in_vertex, 1);
	vec4 p = u_MVP * vec4(in_vertex, 1);
	//vec4 p = vec4(in_vertex, 1);
	gl_Position = p;
	vec4 nn = gl_ModelViewMatrixInverseTranspose * vec4(normalize(in_normal), 0);
	normal = nn.xyz;
	coord = p.xy;
	texcoord = in_texcoord;
}
