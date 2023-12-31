#version 430 core

layout (local_size_x = 32, local_size_y = 32) in;
layout(rgba32f, binding = 0) uniform image2D texture;

void main() {
	ivec2 pixelPos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 screenSize = imageSize(texture);

    vec2 normalizedPos = vec2(pixelPos) / vec2(screenSize);

	imageStore(texture, pixelPos, vec4(normalizedPos.x, 0.0, normalizedPos.y, 1.0));
}