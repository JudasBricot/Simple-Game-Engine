#version 430 core

layout (local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D spectrumBuffer;

const float amplitude = 0.1;
const vec2 wind_direction = vec2(1.0, 0.0);
float wind_amplitude = 31.0;

float gravity = 9.8;

float max_wave_height_sqrd = 31.0 * 31.0 * 31.0 * 31.0 / (gravity * gravity);
float min_wave_height_sqrd = 1.0;

const vec2 step = vec2(2.0f);

uniform float time;

const float PI = 3.141592;

float random (vec2 uv) {
    return fract(sin(dot(uv.xy,
        vec2(12.9898,78.233))) * 43758.5453123);
}

float gaussian (float x) {
    return exp(-x*x / 2) / sqrt(2.0 * PI);
}


void main() {
	ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(spectrumBuffer);

	// Phillips Spectrum generation
	ivec2 centeredPos = ivec2(pixelPos.x - screenSize.x / 2, pixelPos.y - screenSize.y / 2);

	vec2 k = vec2(2.0 * PI * centeredPos.x / (step.x * screenSize.x), 2.0 * PI * centeredPos.y / (step.y * screenSize.y));
	float k_len_sqrd = k.x*k.x + k.y*k.y;

	float k_dir = dot(k, wind_direction);
	float Ph_k = amplitude * k_dir * k_dir * exp(- 1.0 / (k_len_sqrd * max_wave_height_sqrd) - k_len_sqrd * min_wave_height_sqrd) / (k_len_sqrd * k_len_sqrd);
	float Ph_k_sqrt = sqrt(Ph_k);

	// Random Sample
	float theta = random(k);
	float xi = gaussian(random(k.yx));

	vec2 h0_k = xi * vec2(cos(theta), sin(theta)) * Ph_k_sqrt;

	vec3 color = vec3(h0_k.x);

	imageStore(spectrumBuffer, pixelPos, vec4(color, 1.0));
}