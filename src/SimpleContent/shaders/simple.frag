uniform sampler2D diffuseTex;
uniform int uIsCurve;

varying vec3 normal;
varying vec2 texcoord;
in vec2 coord;

void main(void)
{
    //gl_FragColor = vec4(0.5 * normalize(normal) + 0.5, 1.0);
    //gl_FragColor = vec4(texcoord, 0.0, 1.0);
    {
        //gl_FragColor = vec4(0.5 * normalize(normal) + 0.5, 1.0);
        float alpha = smoothstep(0.0, 1.3, length(coord.xy - coord.x));
        gl_FragColor = vec4(1.0, 1.0, 1.0, 0.5);
    }
}
