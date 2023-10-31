uniform sampler2D diffuseTex;

in vec3 local_normal;

void main(void)
{
    gl_FragColor = vec4(0.5 * normalize(local_normal) + 0.5, 1.0);
}
