#version 430 core

layout (local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D colorBuffer;

void main() {
	ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(colorBuffer);

	vec2 pos = vec2(float(pixelPos.x) / screenSize.x - 0.5, float(pixelPos.y) / screenSize.y - 0.5);
	float color = exp(- (pos.x * pos.x + pos.y * pos.y) / (0.2 * 0.2)) / sqrt(2.0 * 3.141592 * 0.2);

	imageStore(colorBuffer, pixelPos, vec4(color, color, color, 1.0));
}