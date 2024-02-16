#version 430 core
#define M_PI 3.1415926535897932384626433832795

layout(local_size_x = 16, local_size_y = 1) in;
layout(binding = 0, rgba32f) writeonly uniform image2D butterflyTexture;

void main() 
{
    int N = imageSize(butterflyTexture).x;
    vec2 x = gl_GlobalInvocationID.xy;

    int n = int(x.x);
    float k = x.x;
    int p = int(x.y) + 1; // stage
    
    int b = N >> p; // Represent the offset between the indices
    int u = b * (n / b); // Index of the partition
	int i = (u + n) % N;

	float phase = 2.0 * M_PI / N * u;
    vec2 twiddle = vec2(cos(phase), sin(phase));

    imageStore(butterflyTexture, ivec2(x), vec4(twiddle.x, twiddle.y, i, i + b));
    imageStore(butterflyTexture, ivec2(x.x + N / 2, x.y), vec4(-twiddle.x, -twiddle.y, i, i + b));
}