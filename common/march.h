

#define MAX_STEPS 1024

float sdf(vec3 pos) {
	return length(pos) - 1.0;
}

struct MarchData {
	float t;
	vec3 position;
	int steps;
};

MarchData march(vec3 start, vec3 end) {

	MarchData data;
	data.position = start;
	float max_t = length(end - start);
	vec3 direction = (end - start) / max_t; // Normalized direction
	float d = sdf(start);

	while (d > FLT_EPSILON) {

		data.position += direction * d;
		d = sdf(data.position);

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

	return data;

}