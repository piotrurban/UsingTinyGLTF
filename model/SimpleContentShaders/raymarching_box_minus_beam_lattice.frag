in vec2 coord;
out vec4 color;

uniform float u_rayCos;
uniform mat4 u_MVP;
uniform mat4 u_InverseMVP;
uniform float u_maxCount;

vec3 gradient(vec3 delta_r, float f_r, float f_dx, float f_dy, float f_dz);
float sierp_parallelogram(vec3 p, vec3 b0, vec3 b, float iter);

// Calcs intersection and exit distances, and normal at intersection.
// The ray must be in box/object space. If you have multiple boxes all
// aligned to the same axis, you can precompute 1/rd. If you have
// multiple boxes but they are not alligned to each other, use the 
// "Generic" box intersector bellow this one.
vec2 boxIntersection( in vec3 ro, in vec3 rd, in vec3 rad, out vec3 oN ) 
{
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;

    float tN = max( max( t1.x, t1.y ), t1.z );
    float tF = min( min( t2.x, t2.y ), t2.z );
	
    if( tN>tF || tF<0.0) return vec2(-1.0); // no intersection
    
    oN = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);

    return vec2( tN, tF );
}

vec2 iBox( in vec3 ro, in vec3 rd, in vec3 rad, out vec3 oN ) 
{
    vec3 m = 1.0/rd;
    vec3 n = m*ro;
    vec3 k = abs(m)*rad;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
	oN = -sign(rd)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
	
	return vec2( max( max( t1.x, t1.y ), t1.z ),
	             min( min( t2.x, t2.y ), t2.z ) );
}

float sphere(vec3 p, vec3 a, float r)
{
	//return length(p - a) - r;
	return length(max(abs(p) - a,0.0)) - r;
}

float centered_parallelogram(vec3 p, vec3 a, float r)
{
	return length(max(abs(p) - a,0.0)) - r;
}

float parallelogram(vec3 p, vec3 b0, vec3 b, float r)
{
	vec3 p_tr = p - b0;
	vec3 q = abs(p_tr) - b;
	return length(max(q, 0.0)) - r;
}

float sierp_parallelogram(vec3 p, vec3 b0, vec3 b, float iter)
{
	float dist = 10.0;
	vec3 p_tr = abs(p - b0);
	float it = 0.0;
	while (min(p_tr.x/b.x, min(p_tr.y/b.y, p_tr.z/b.z)) > 1.0/3.0)
	{
		it += 1;
		p_tr = 1.5*(p_tr - 1.0/3.0*b);
		if (it > iter )
		{
			break;
		}
	}
	vec3 b3 = b/3.0;
	return min(p_tr.x/b.x, min(p_tr.y/b.y, p_tr.z/b.z)) - 0.02;

}

float corner_frame(vec3 p, vec3 sides, float r)
{
	float dist_to_segment = length(max(abs(p-vec3(0.0,sides.yz)) - vec3(sides.x, 0,0),0.0));
	dist_to_segment = min(dist_to_segment, length(max(abs(p-vec3(sides.x, 0, sides.z)) - vec3(0,sides.y,0), 0.0)));
	dist_to_segment = min(dist_to_segment, length(max(abs(p-vec3(sides.x, sides.y,0)) - vec3(0,0,sides.z),0.0)) );
	return dist_to_segment - r;
}

float sdfBox(vec3 p, vec3 halfSides, float r)
{
	vec3 q = abs(p) - halfSides;
	return length(max(q,0)) + min(max(q.x, max(q.y,q.z)),0) - r;
}

float sdfPole(vec3 p, vec2 halfSides, float r)
{
	vec2 q = abs(p.xy) - halfSides;
	return length(max(q,0)) + min(max(q.x,q.y),0) - r;
}

float sdfCross(vec3 p, vec3 halfSides, float r)
{
	float dist = sdfPole(p, halfSides.xy, r);
	dist = min(dist, sdfPole(p.xzy, halfSides.xz, r));
	dist = min(dist, sdfPole(p.yzx, halfSides.yz, r));
	return dist;
}

float sdfDrilledCube(vec3 p, float side, float r, out vec3 extra)
{
	float dist = sdfBox(p, vec3(0.5*side), r);
	float divider = 1.0;
	for(int iter=0; iter<3; iter++)
	{
		dist = max(dist, -sdfCross(p, vec3(0.16666*side), r));
		p = fract(p*3.0/side - 1.0)*side;
		divider *= 3.0;
	}
	extra = vec3(divider,0,0);
	return dist;
}

float sdfCubeCrosses(vec3 p, float side, float r, out vec3 extra)
{
	float dist = 10000.0;
	float divider = 1.0;
	for(int iter=0; iter<2; iter++)
	{
		vec3 pdivs = floor(abs(p)/side);
		if (length(pdivs) >2)
		{
			break;
		}
		dist = min(dist, sdfCross(p, vec3(0.16666*side), r));
		p = fract(p*3.0/side - 1.0)*side;
		divider *= 3.0;
	}
	extra = vec3(divider,0,0);
	return dist;
}

float sdfBeamLattice(vec3 p, float beamHalfSide, float beamSeparation, float r)
{
	vec3 p_reduced =  p - beamSeparation*floor(p/beamSeparation + vec3(0.5));
	vec3 qXY = abs(p_reduced) - vec3(beamHalfSide, beamHalfSide, beamSeparation);
	vec3 qYZ = abs(p_reduced) - vec3(beamSeparation, beamHalfSide, beamHalfSide);
	vec3 qXZ = abs(p_reduced) - vec3(beamHalfSide, beamSeparation, beamHalfSide);
	float distXY = length(max(qXY,0)) + min(max(max(qXY.x,qXY.y),qXY.z),0);
	float distYZ = length(max(qYZ,0)) + min(max(max(qYZ.x,qYZ.y),qYZ.z),0);
	float distXZ = length(max(qXZ,0)) + min(max(max(qXZ.x,qXZ.y),qXZ.z),0);
	return min(distXZ, min(distXY, distYZ)) - r;
}

float sdfBoxMinusBeamLattice(vec3 p, vec3 b, float beamHalfSide, float beamSeparation, float r)
{
	float dist = sdfBox(p, b, r);
	float divisor = 1.0;
	for (int i = 0; i < 5; i++)
	{
		dist = max(dist, - sdfBeamLattice(p, beamHalfSide/divisor, beamSeparation/divisor, r));
		divisor *= 3.0;
	}
	return dist;
}

float general_figure(vec3 p, vec3 v_param1, vec3 v_param2, float f_param1, float f_param2, float r, out vec3 extra)
{
	//return parallelogram(p, v_param1, v_param2, r);
	//return sierp_parallelogram(p, v_param1, v_param2, f_param1);
	//return corner_frame(p, v_param1, r);
	//return sdfDrilledCube(p, v_param1.x, r, extra);
	//return sdfCubeCrosses(p, v_param1.x, r, extra);
	//return sdfBeamLattice(p, v_param1.x, v_param1.y, r);
	return sdfBoxMinusBeamLattice(p, v_param1, v_param2.x, v_param2.y, r);
}

vec4 raymarch_general_figure(vec3 ro, vec3 rd, vec3 v_param1, vec3 v_param2, float f_param1, float f_param2, float r)
{
	float eps = 0.000015;
	float D = 10.0;
	float dist = 0.0;
	vec3 rd_n = normalize(rd);
	vec3 grad = vec3(0.0);
	vec3 delta_r = vec3(0.00001, 0.00001, 0.00001);
	vec3 extra;
	int count = 0;
	while ( dist < D)
	{
		count++;
		vec3 pos = ro + rd*dist;
		vec4 posMVP = u_InverseMVP * vec4(pos, 1.0);
		pos = (posMVP/posMVP.w).xyz;

		float f_r = general_figure(pos, v_param1, v_param2, f_param1, f_param2, r, extra);
		float f_dx = general_figure(pos + vec3(delta_r.x, 0.0,  0.0), v_param1, v_param2, f_param1, f_param2, r, extra);
		float f_dy = general_figure(pos + vec3(0.0,  delta_r.y, 0.0), v_param1, v_param2, f_param1, f_param2, r, extra);
		float f_dz = general_figure(pos + vec3(0.0,  0.0,  delta_r.z), v_param1, v_param2, f_param1, f_param2, r, extra);
		grad = gradient(delta_r, f_r, f_dx, f_dy, f_dz);
		if (f_r < eps)
		{
			return vec4(grad, float(count));	
		}
		//float step = - f_r/dot(rd_n, grad);
		float step = f_r/length(rd);
		if (step < 0.0)
		{
			return vec4(1.0,1.0,1.0,0.0);
			break;
		}
		dist += step;
	}
	discard;
	return vec4(0.0, dist/D, 1.0,0.0);

}

vec4 raycast_box(vec3 ro, vec3 rd, vec3 b)
{
	vec3 norm;
	vec2 intersection = iBox(ro, rd, b, norm);
	if (intersection.x < intersection.y)
	{
		return vec4(norm, intersection.x);
	}
	else
	{
		discard;
	}
}

vec3 gradient(vec3 delta_r, float f_r, float f_dx, float f_dy, float f_dz)
{
	return vec3((f_dx - f_r)/delta_r.x, (f_dy - f_r)/delta_r.y, (f_dz - f_r)/delta_r.z);
}

float test_x(vec3 r)
{
	return r.x*r.x;
}

vec3 raymarch_sphere(vec3 ro, vec3 rd, vec3 center, float radius)
{
	float eps = 0.015;
	float D = 100.0;
	float dist = 0.0;
	vec3 rd_n = normalize(rd);
	vec3 grad = vec3(0.0);
	vec3 delta_r = vec3(0.0001, 0.0001, 0.0001);
	while ( dist < D)
	{
		vec3 pos = ro + rd*dist;
		float f_r = sphere(pos, center, radius);
		float f_dx = sphere(pos + vec3(delta_r.x, 0.0,  0.0), center, radius);
		float f_dy = sphere(pos + vec3(0.0,  delta_r.y, 0.0), center, radius);
		float f_dz = sphere(pos + vec3(0.0,  0.0,  delta_r.z), center, radius);
		grad = gradient(delta_r, f_r, f_dx, f_dy, f_dz);
		//return grad;
		if (f_r < eps)
		{
			//return vec3(1.0,0.5,0.0);
			return grad;	
		}
		float step = - f_r/dot(rd_n, grad);
		if (step < 0.0)
		{
			break;
		}
		dist += step;
	}
	discard;
	return vec3(0.0,0.0,0.0);

}

void main()
{
	vec3 ro = vec3(coord, -5.0);
	vec3 rd = vec3(0.01, 0.01, 1.0);
	float rounding = 0.0010;

	vec4 ro4 = u_InverseMVP*vec4(ro,1.0);
	vec4 rd4 = u_InverseMVP*vec4(rd,1.0);
	ro = ro4.xyz/ro4.w;
	rd = rd4.xyz/rd4.w;
	vec4 gr_f = raycast_box(ro, rd, vec3(0.2,0.2,0.2));
	vec3 gr = gr_f.xyz;
	float dist = gr_f.w;
	vec3 bkg_color= vec3(0.8,0.8,0.5);
	vec3 lighting_color = vec3(1.0,0.0,0.0);
	vec3 lighting_color_2 = vec3(0.0,1.0,0.0);
	vec3 lighting_color_3 = vec3(0.0,0.0,1.0);
	
	//color = vec4( bkg_color + max(vec3(0.0),pow(gr3.x,3)* lighting_color_2) + max(vec3(0.0), pow(gr3.y,3) * lighting_color) + 0.2*max(vec3(0.0), pow(gr3.z,3)*lighting_color_3), 1.0);
	color = vec4(  lighting_color_2, 1.0);
}