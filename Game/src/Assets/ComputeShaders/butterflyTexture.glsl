#version 430 core
#define M_PI 3.1415926535897932384626433832795

layout(local_size_x = 1, local_size_y = 16) in;
layout(binding = 0, rgba32f) writeonly uniform image2D butterflyTexture;
layout(std430, binding = 1) buffer indices {
    int bit_reversed[];
};

void main(void) 
{
    int N = imageSize(butterflyTexture).x;
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);

    float k = mod(x.x * (float(N) / pow(2, x.y + 1)), float(N));
    vec2 twiddle = vec2(cos(2.0 * M_PI * k / float(N)), sin(2.0 * M_PI * k / float(N)));
    int butterflySpan = int(pow(2, x.y));
    int butterflyWing;

    if (mod(x.x, pow(2, x.y + 1)) < pow(2, x.y))
        butterflyWing = 1;
    else
        butterflyWing = 0;

    // First stage, bit-reversed indices
    if (x.y == 0) {
        // Top butterfly wing
        if (butterflyWing == 1)
            imageStore(butterflyTexture, ivec2(x), vec4(twiddle.x, twiddle.y, bit_reversed[int(x.x)], bit_reversed[int(x.x + 1)]));
        // Bottom butterfly wing
        else
            imageStore(butterflyTexture, ivec2(x), vec4(twiddle.x, twiddle.y, bit_reversed[int(x.x - 1)], bit_reversed[int(x.x)]));
    }
    // Second to log2(N) stage
    else {
        // Top butterfly wing
        if (butterflyWing == 1)
            imageStore(butterflyTexture, ivec2(x), vec4(twiddle.x, twiddle.y, x.x, x.x + butterflySpan));
        // Bottom butterfly wing
        else
            imageStore(butterflyTexture, ivec2(x), vec4(twiddle.x, twiddle.y, x.x - butterflySpan, x.x));
    }
}