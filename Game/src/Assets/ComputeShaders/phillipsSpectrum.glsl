#version 430 core

layout (local_size_x = 16, local_size_y = 16) in;
layout(rgba32f, binding = 0) uniform image2D spectrumBuffer;
layout(rgba32f, binding = 1) uniform image2D slopeBuffer;
layout(rgba32f, binding = 2) uniform image2D displacementBuffer;

layout(std430, binding = 3) buffer globalParam {
	vec2 step;
	vec2 wind_direction;
	float wind_amplitude;
	float gravity;
	float time;
	int spectrumIndex;
	int directionalSpreadingIndex;
};

layout(std430, binding = 4) buffer _Phillips {
    float amplitude;
	float max_wave_height_sqrd;
	float min_wave_height_sqrd;
	float alignementFactor;
} phillipsParams;

layout(std430, binding = 5) buffer _Jonswap {
	float amplitude;
    float fetch;
	float gamma;

} jonswapParams;

layout(std430, binding = 6) buffer _Pierson {
	float amplitude;

} piersonParams;

/*layout(std430, binding = 7) buffer _TMA {
	bool useTMA;
	float depth;
} TMAParams;*/

const float PI = 3.1415926535897932384626433832795f;

float hash(float x)
{
    // based on: pcg by Mark Jarzynski: http://www.jcgt.org/published/0009/03/02/
    uint state = uint(x * 8192.0) * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return float((word >> 22u) ^ word) * (1.0 / float(0xffffffffu));
}

vec2 UniformToGaussian(float u1, float u2) {
    float R = sqrt(-2.0f * log(u1));
    float theta = 2.0f * PI * u2;

    return vec2(R * cos(theta), R * sin(theta));
}

// =============================
//    Non-Directional Spectra 
// =============================

// --------------
//    Phillips 
// --------------

float PhillipsSpectrum(float amplitude, vec2 k_dir, float k_len_sqrd, float g, float l_sqrd, float L_sqrd, float alignementFactor)
{	
	// Remove the directional part;
	//float dot_k_w = pow(dot(k_dir, wind_direction), alignementFactor);
	
	float Ph_k = amplitude /** dot_k_w*/ * exp(- 1.0 / (k_len_sqrd * L_sqrd) - k_len_sqrd * l_sqrd) / (k_len_sqrd * k_len_sqrd);

	return Ph_k;
}

// -------------
//    JONSWAP 
// -------------

float Sigma(float omega, float omega_p)
{
	if(omega <= omega_p)
		return 0.07;
	return 0.09;
}

float Omega_p(float g, float wind_speed, float fetch)
{
	return 22.0 * pow((g*g / wind_speed / fetch), 0.3);
}

float Alpha(float g, float wind_speed, float fetch)
{
	return 0.076 * pow(wind_speed * wind_speed / (fetch * g), 0.22);
}

float R(float omega, float omega_p, float sigma)
{
	return exp(- 0.5 * pow((omega - omega_p) / sigma / omega_p, 2.0));
}

float JonswapSpectrum(float amplitude, float g, float omega, float gamma, float wind_speed, float fetch)
{
	float alpha = Alpha(g, wind_speed, fetch);
	float omega_p = Omega_p(g, wind_speed, fetch);
	float sigma = Sigma(omega, omega_p);
	float r = R(omega, omega_p, sigma);

	return 500.0 * amplitude * alpha * g * g / pow(omega, 5.0) * exp(- 1.25 * pow(omega_p / omega, 4.0)) * pow(gamma, r);
}

// -------------
//    Pierson 
// -------------

float PiersonSpectrum(float amplitude, float omega, float g, float wind_speed)
{
	float alpha = 0.0081;
	float beta = 0.74;
	float omega_0 = g / (1.026 * wind_speed);

	return 400.0 * amplitude * alpha * g * g / pow(omega, 5.0) * exp(- beta * pow(omega_0 / omega, 4.0));
}


float NonDirectionalSpectrum(vec2 k_dir, float k_len_sqrd, float omega, float g, float wind_speed)
{
	float nonDirSpectrum;
	
	// Variable change
	float d_omega_d_k = 0.5 * omega / k_len_sqrd;

	switch(spectrumIndex)
	{
		case 0:
			nonDirSpectrum = PhillipsSpectrum(phillipsParams.amplitude, k_dir, k_len_sqrd, g, phillipsParams.min_wave_height_sqrd, phillipsParams.max_wave_height_sqrd, phillipsParams.alignementFactor);
			break;
		case 1:
			nonDirSpectrum = JonswapSpectrum(jonswapParams.amplitude, g, omega, jonswapParams.gamma, wind_speed, jonswapParams.fetch) * d_omega_d_k;
			break;
		case 2:
			nonDirSpectrum = PiersonSpectrum(piersonParams.amplitude, omega, g, wind_speed) * d_omega_d_k;
			break;
	}
	
	return nonDirSpectrum;
}

/*float TMA(float k_len, float g, float depth)
{
	float omega = sqrt(g * k_len);
	float omega_h = omega * sqrt(depth / g);

	if(omega_h <= 1.0)
		return 0.5 * omega_h*omega_h;
	return 1.0 - 0.5 * (2.0 - omega_h) * (2.0 - omega_h);
}*/

// =============================
//    Directional Spectra 
// =============================

float PhillipsDirectional(float theta)
{
	return pow(cos(theta), phillipsParams.alignementFactor);
}

float PositiveCosine(float theta)
{
	float c = max(cos(theta), 0.0);

	return 2.0 / PI * c * c;
}

float DonelanBanner(float omega, float theta, float g, float wind_speed)
{
	float omega_p = Omega_p(g, wind_speed, jonswapParams.fetch);
	float omega_over_omega_p = omega / omega_p;
	float epsilon = -0.4 + 0.8393 * exp(-0.567 * log(pow(omega_over_omega_p, 2.0)));

	float beta_s;
	if(omega_over_omega_p < 0.56)
		beta_s = 0.0;
	else if(omega_over_omega_p < 0.95)
		beta_s = 2.61 * pow(omega_over_omega_p, 1.3);
	else if(omega_over_omega_p < 1.6)
		beta_s = 2.28 * pow(omega_over_omega_p, -1.3);
	else
		beta_s = pow(10.0, epsilon);

	return beta_s / (2.0 * tanh(beta_s * PI)) / (cosh(beta_s * theta) * cosh(beta_s * theta));
}

float DirectionalSpreading(float omega, float theta, float g, float wind_speed)
{
	switch(directionalSpreadingIndex)
	{
		case 0:
			return 2.0 / PI;
		case 1:
			return PhillipsDirectional(theta);
		case 2:
			return PositiveCosine(theta);
		case 3:
			return DonelanBanner(omega, theta, g, wind_speed);
	}
}

float GlobalSpectrum(vec2 k_dir, float k_len_sqrd, float g, float wind_speed)
{
	float k_len = sqrt(k_len_sqrd);
	float omega = sqrt(g * k_len);
	float theta = atan(k_dir.y / k_dir.x) - acos(wind_direction.x);

	float spectrum = NonDirectionalSpectrum(k_dir, k_len_sqrd, omega, gravity, wind_amplitude);
	spectrum *= DirectionalSpreading(omega, theta, g, wind_speed);

	/*if(TMAParams.useTMA)
		spectrum *= TMA(k_len, g, TMAParams.depth);*/

	return sqrt(spectrum);
}

vec4 SpectrumAmplitude(vec2 k_dir, float k_len_sqrd, int seed)
{
	vec4 randomSamples = vec4(hash(seed), hash(2 * seed), hash(3 * seed), hash(4 * seed));

	float spectrum = GlobalSpectrum(k_dir, k_len_sqrd, gravity, wind_amplitude);

	vec2 epsilon_0 = UniformToGaussian(randomSamples.x, randomSamples.y);
	vec2 epsilon_1 = UniformToGaussian(randomSamples.z, randomSamples.w);

	float invert_two_sqrt = 1.0 / sqrt(2.0);

	vec2 h0_k_0 = invert_two_sqrt * epsilon_0 * spectrum;
	vec2 h0_k_1 = invert_two_sqrt * epsilon_1 * spectrum;

	return vec4(h0_k_0, h0_k_1);
} 

vec2 FourierAmplitude(vec2 k_dir, float k_len_sqrd, int seed, float g, float t)
{
	vec4 h0 = SpectrumAmplitude(k_dir, k_len_sqrd, seed);

	float omega = sqrt(g * sqrt(k_len_sqrd) + k_len_sqrd * 0.01);
	vec2 cosin = vec2(cos(omega * t), sin(omega * t));
	
	return vec2(h0.x + h0.z, h0.y - h0.w) * cosin;
}

void main() 
{
	ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(spectrumBuffer);
	vec2 centeredPos = vec2(float(pixelPos.x) - float(screenSize.x) / 2.0, float(pixelPos.y) - float(screenSize.y) / 2.0);

	int seed = int(pixelPos.x) + int(pixelPos.y) * screenSize.x + screenSize.y;

	vec2 dk = 2.0 * PI * vec2 (1.0 / (step.x * float(screenSize.x)), 1.0 / (step.y * float(screenSize.y)));
	vec2 k = dk * centeredPos;
	vec2 k_dir = normalize(k);
	float k_len_sqrd = k.x*k.x + k.y*k.y;

	vec2 h0 = FourierAmplitude(k_dir, k_len_sqrd, seed, gravity, time);
	vec2 ih = vec2(-h0.y, h0.x);
	vec2 slope = vec2(ih.x * k.x - ih.y * k.y, ih.y * k.x + ih.x * k.y);
	vec2 displacement = vec2(ih.x * k_dir.x - ih.y * k_dir.y, ih.y * k_dir.x + ih.x * k_dir.y);

	vec3 color = vec3(h0  / (step.x * step.y), 0.0);

	imageStore(spectrumBuffer, ivec2(pixelPos), vec4(color, 1.0));
	imageStore(slopeBuffer, ivec2(pixelPos), vec4(100.0 * slope, 0.0, 1.0));
	imageStore(displacementBuffer, ivec2(pixelPos), vec4(displacement, 0.0, 1.0));
}