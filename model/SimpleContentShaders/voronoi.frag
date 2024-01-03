in vec2 coord;
out vec4 color;

uniform float u_cameraZ;

void main()
{
	vec2 pos= mod(coord.xy, 1.0);
	vec3 p1 = vec3(0.3, 0.4, 0.1);	
	vec3 p2 = vec3(-0.3+u_cameraZ*0.01, 0.2, 0.1);	
	vec3 p3 = vec3(0.3, -0.3, 0.1);	
	vec3 p4 = vec3(0.0, 0.6, 0.1);
	vec3 ps[4] = vec3[4](p1,p2,p3,p4);
	float dist = 1000.0;
	int j=-1;
	for (int i = 0; i < 4; i++)
	{
		float d = length(pos - ps[i].xy);
		if (d < dist)
		{
			j=i;
			dist = d;
		}
	}
	if (j==-1)
	{
		color=vec4(1.0,0.0,0.0,1.0);
	}
	else
	{
		color=vec4(0.3*j,0.25*j+0.25, 0.01*j*j +0.1*j, 1.0 -dist );
	}
}