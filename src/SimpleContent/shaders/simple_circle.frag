uniform sampler2D diffuseTex;
uniform float u_radius;

varying vec3 normal;
varying vec2 texcoord;
in vec2 coord;

void main(void)
{
    //gl_FragColor = vec4(0.5 * normalize(normal) + 0.5, 1.0);
    //gl_FragColor = vec4(texcoord, 0.0, 1.0);
    {
        //gl_FragColor = vec4(0.5 * normalize(normal) + 0.5, 1.0);
        float alpha = smoothstep(u_radius - 0.1, u_radius + 0.1, length(coord.xy));
        gl_FragColor = vec4(1.0, 1.0, 1.0,  1.0 -alpha);
    }
}
