uniform sampler2D diffuseTex;
uniform float u_radius;

varying vec3 normal;
varying vec2 texcoord;
in vec2 coord;

float signed_distance_from_line(vec3 a, vec3 b, vec3 p)
{
    vec3 AB =  b - a;
    vec3 normalAB = normalize(vec3(-AB.y, AB.x, 0.0)); // assume a.z == b.z
    vec3 AP = p - a;
    return -dot(AP, normalAB);
}

float rounded_distance_from_line(vec3 a, vec3 b, vec3 p)
{
    vec3 AB =  b - a;
    vec3 normalAB = normalize(vec3(-AB.y, AB.x, 0.0)); // assume a.z == b.z
    vec3 AP = p - a;
    float signed_dist =  -dot(AP, normalAB);
    if (true) // (signed_dist > 0.0)
    {
        signed_dist = max(signed_dist, 0.15*length(p-a));
        signed_dist = max(signed_dist, 0.15*length(p-b));
    }
    return signed_dist;
}

void main(void)
{
        vec3 a = vec3(-0.5, 0.0, 0.0);
        vec3 b = vec3(0.5, 0.5, 0.0);
        vec3 c = vec3(0.0, 1.0, 0.0);
        vec3 d = vec3(-1.0, 0.5, 0.0);
        vec3 coord3 = vec3(coord, 0.0);
        float dist = rounded_distance_from_line(a,b, coord3);
        dist = max(dist, rounded_distance_from_line(b, c, coord3));
        dist = max(dist, rounded_distance_from_line(c, d, coord3));
        dist = max(dist, rounded_distance_from_line(d, a, coord3));
        //gl_FragColor = vec4(0.5 * normalize(normal) + 0.5, 1.0);
        float alpha = smoothstep(-0.85,0.85, -dist);
        vec3 color = vec3(cos(5.0*coord.x), sin(coord.y), tan(2.0*coord.y*coord.x));
        gl_FragColor = vec4(color, alpha);
}
