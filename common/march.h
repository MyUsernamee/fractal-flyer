

#define SDF_SPHERE 0
#define SDF_CUBE 1
#define SDF_SPONGE 2

#define INTERSECTION_UNION 0
#define INTERSECTION_SUBTRACT 1
#define INTERSECTION_INTERSECT 2

#define MAX_STEPS 1024
#define EPSILON 0.001
#define INF 1e10

float mandelbulb(vec3 pos) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    int iterations = 10;
    float power = 8.0;

    for (int i = 0; i < iterations; i++) {
        r = length(z);
        if (r > 2.0) break;

        // convert to polar coordinates
        float theta = acos(z.z/r);
        float phi = atan(z.y, z.x);
        dr = pow(r, power - 1.0) * power * dr + 1.0;

        // scale and rotate the point
        float zr = pow(r, power);
        theta *= power;
        phi *= power;

        // convert back to cartesian coordinates
        z = zr * vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
        z += pos;
    }

    return 0.5 * log(r) * r / dr;
}

float cube_sdf(vec3 p){
	return max(abs(p.x), max(abs(p.y), abs(p.z))) - 0.5;
}

float cross_sdf(vec3 p) {
	return min(min(max(abs(p.x), abs(p.y)) - 0.5f, max(abs(p.y), abs(p.z)) - 0.5f), max(abs(p.z), abs(p.x)) - 0.5f);
}

float mengerSponge1(vec3 p) {

	return max(-cross_sdf(p * 3.0f) / 3.0f, cube_sdf(p));

}

const float pi = 3.14159265359;

float sdBox( vec3 p, vec3 b )
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0f)) + min(max(q.x,max(q.y,q.z)),0.0f);
}
float sdSphere(vec3 p, float r){
    return length(p) -r;
}

mat2 rotate(float a){
    float s = sin(a);
    float c = cos(a);
    return mat2(c, s,
                -s, c);
}

vec3 trans(vec3 p, float s){
        //Mirror
        p = abs(p)-1.f*s;
        p *= -1.;
		// Reflect column
		if (p.x - p.y > 0.0) {
			float temp = p.x;
			p.x = p.y;
			p.y = temp;
		}
		if (p.z - p.y > 0.0) {
			float temp = p.z;
			p.z = p.y;
			p.y = temp;
		}  //construct column
        p.y = (abs(p.y-0.5*s)-0.5*s);
        
        return p;
}


// I don't know why but putting the calls to trans in a loop caused some weid behavioirs
float map(vec3 p) {

    const float scale = 260.;

    p*= scale;
    
    #ifdef rotations
    p = trans(p, 27.*9.);
    p.xy *= rotate(13.2);
    p = trans(p, 27.*3.);
    p.zy *= rotate(1.2);
    p = trans(p, 27.);
    p.xz *= rotate(13.12);
    p = trans(p, 9.);
    p.yz *= rotate(1.12);
    p = trans(p, 3.);
    p.yz *= rotate(6.12);
    p = trans(p, 1.);
    #else
    p = trans(p, 27.*9.);
    p = trans(p, 27.*3.);
    p = trans(p, 27.);
    p = trans(p, 9.);
    p = trans(p, 3.);
    p = trans(p, 1.);
    
    #endif

    return sdBox(p, vec3(.5))/scale - 0.0005;
}

// Example usage as your main SDF function:
float sdf(vec3 pos, int sdf_type) {

	switch(sdf_type){
		case SDF_SPHERE:
			return sdSphere(pos, 1.0);
			break;
		case SDF_CUBE:
			return cube_sdf(pos);
			break;
		case SDF_SPONGE:
			return map(pos);
			break;
		default:
			return INF;
			break;
	}

}

float intersection_type(float a, float b, int intersection_type) {
	switch(intersection_type) {
		case INTERSECTION_UNION:
			return min(a, b);
		case INTERSECTION_SUBTRACT:
			return max(-a, b);
		case INTERSECTION_INTERSECT:
			return max(a, b);
		default:
			return INF;
	}
}

float object_sdf(vec3 pos, Object object) {

	vec3 local_position = pos - vec3(object.model_matrix[3]);
	float d = sdf(vec3(object.model_matrix * vec4(pos, 1.0)), object.sdf_type) / (length(local_position) / length(local_position * mat3(object.model_matrix)));

	return d;
}

float full_sdf(vec3 pos, Object objects[MAX_OBJECTS], int count) {

	float d = object_sdf(pos, objects[0]);

	for (int i = 1; i < count; i++) {

		d = intersection_type(object_sdf(pos, objects[i]), d, objects[i].intersection_type);

	}

	return d;

}

struct MarchData {
	float t;
	vec3 position;
	int steps;
	bool intersection;
};

MarchData march(vec3 start, vec3 end, Object objects[MAX_OBJECTS], int count) {

	MarchData data;
	data.intersection = false;
	data.position = start;
	float max_t = length(end - start);
	vec3 direction = (end - start) / max_t; // Normalized direction
	float d = full_sdf(start, objects, count);

	while (d > EPSILON) {

		data.position += direction * d;
		d = full_sdf(data.position, objects, count);

		data.t += d;
		data.steps += 1;		

		if (data.t >= max_t) {
			data.position = end;
			return data; // Reached the end without hitting anything
		}

		if (data.steps > MAX_STEPS) {
			return data;
		}

	}

	data.intersection = true;
	return data;

}

vec3 get_normal(vec3 position, int sdf_type) {

	float dx = sdf(position + vec3(EPSILON, 0.0, 0.0), sdf_type) - sdf(position - vec3(EPSILON, 0.0, 0.0), sdf_type);
	float dy = sdf(position + vec3(0.0, EPSILON, 0.0), sdf_type) - sdf(position - vec3(0.0, EPSILON, 0.0), sdf_type);
	float dz = sdf(position + vec3(0.0, 0.0, EPSILON), sdf_type) - sdf(position - vec3(0.0, 0.0, EPSILON), sdf_type);
	return normalize(vec3(dx, dy, dz));

}

vec3 get_world_normal(vec3 position, Object objects[MAX_OBJECTS], int count) {

	float dx = full_sdf(position + vec3(EPSILON, 0.0, 0.0), objects, count) - full_sdf(position - vec3(EPSILON, 0.0, 0.0), objects, count);
	float dy = full_sdf(position + vec3(0.0, EPSILON, 0.0), objects, count) - full_sdf(position - vec3(0.0, EPSILON, 0.0), objects, count);
	float dz = full_sdf(position + vec3(0.0, 0.0, EPSILON), objects, count) - full_sdf(position - vec3(0.0, 0.0, EPSILON), objects, count);
	return normalize(vec3(dx, dy, dz));



}