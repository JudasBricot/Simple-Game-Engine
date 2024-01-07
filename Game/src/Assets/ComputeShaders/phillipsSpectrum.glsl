#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform image2D spectrumBuffer;
layout(rgba32f, binding = 1) uniform image2D slopeBuffer;
layout(rgba32f, binding = 2) uniform image2D displacementBuffer;

layout(std430, binding = 3) buffer ssbo {
	vec2 step;
	vec2 wind_direction;
    float amplitude;
	float wind_amplitude;

	float gravity;

	float max_wave_height_sqrd;
	float min_wave_height_sqrd;

	float time;
};

const float PI = 3.1415926535897932384626433832795f;

float hash(float x)
{
    // based on: pcg by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
    uint state = uint(x * 8192.0) * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return float((word >> 22u) ^ word) * (1.0 / float(0xffffffffu));;
}

vec2 UniformToGaussian(float u1, float u2) {
    float R = sqrt(-2.0f * log(u1));
    float theta = 2.0f * PI * u2;

    return vec2(R * cos(theta), R * sin(theta));
}

float PhillipsSpectrumSqrt(vec2 k_dir, float k_len_sqrd)
{	
	float dot_k_w = pow(dot(k_dir, wind_direction), 4.0);
	
	float Ph_k = amplitude * dot_k_w * dot_k_w * exp(- 1.0 / (k_len_sqrd * max_wave_height_sqrd) - k_len_sqrd * min_wave_height_sqrd) / (k_len_sqrd * k_len_sqrd);
	float Ph_k_sqrt = sqrt(Ph_k);

	return Ph_k_sqrt;
}

vec4 SpectrumAmplitude(vec2 k_dir, float k_len_sqrd, int seed)
{
	vec4 randomSamples = vec4(hash(seed), hash(2 * seed), hash(3 * seed), hash(4 * seed));

	float Ph_k_sqrt = PhillipsSpectrumSqrt(k_dir, k_len_sqrd);

	vec2 epsilon_0 = UniformToGaussian(randomSamples.x, randomSamples.y);
	vec2 epsilon_1 = UniformToGaussian(randomSamples.z, randomSamples.w);

	float invert_two_sqrt = 1.0 / sqrt(2.0);

	vec2 h0_k_0 = invert_two_sqrt * epsilon_0 * Ph_k_sqrt;
	vec2 h0_k_1 = invert_two_sqrt * epsilon_1 * Ph_k_sqrt;

	return vec4(h0_k_0, h0_k_1);
}

vec2 FourierAmplitude(vec2 k_dir, float k_len_sqrd, int seed, float g, float t)
{
	vec4 h0 = SpectrumAmplitude(k_dir, k_len_sqrd, seed);

	float omega = sqrt(g * sqrt(k_len_sqrd) + k_len_sqrd * 0.01);
	vec2 cosin = vec2(cos(omega * t), sin(omega * t));
	
	return vec2(h0.x + h0.z, h0.y - h0.w) * cosin;
}

const float L = 1000.0;

void main() 
{
	ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(spectrumBuffer);
	vec2 centeredPos = vec2(float(pixelPos.x) - float(screenSize.x) / 2.0, float(pixelPos.y) - float(screenSize.y) / 2.0);

	int seed = int(pixelPos.x) + int(pixelPos.y) * screenSize.x + screenSize.y;

	vec2 k = vec2(2.0 * PI * centeredPos.x / (step.x * float(screenSize.x)), 2.0 * PI * centeredPos.y / (step.y * float(screenSize.y)));
	vec2 k_dir = normalize(k);
	float k_len_sqrd = k.x*k.x + k.y*k.y;

	vec2 h0 = FourierAmplitude(k_dir, k_len_sqrd, seed, gravity, time);
	vec2 ih = vec2(-h0.y, h0.x);
	vec2 slope = vec2(ih.x * k.x - ih.y * k.y, ih.y * k.x + ih.x * k.y);
	vec2 displacement = - vec2(ih.x * k_dir.x - ih.y * k_dir.y, ih.y * k_dir.x + ih.x * k_dir.y);

	vec3 color = vec3(h0, 0.0);

	imageStore(spectrumBuffer, ivec2(pixelPos), vec4(color, 1.0));
	imageStore(slopeBuffer, ivec2(pixelPos), vec4(100.0 * slope, 0.0, 1.0));
	imageStore(displacementBuffer, ivec2(pixelPos), vec4(displacement, 0.0, 1.0));
}