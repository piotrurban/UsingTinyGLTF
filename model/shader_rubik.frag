#version 450
uniform sampler2D diffuseTex;

in vec3 local_normal;
out vec4 color;

void main(void)
{
    color = vec4(0.5 * normalize(local_normal) + 0.5, 1.0);
}
