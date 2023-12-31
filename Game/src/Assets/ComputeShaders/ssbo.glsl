#version 430 core

layout(local_size_x=16,local_size_y=16) in;
layout(rgba32f, binding= 0) uniform writeonly image2D texture;

layout(std430, binding = 1) buffer ssbo {
    vec4 i_Color;
	float i_Time;
};

void main()
{
	/*vec2 coords = vec2(gl_GlobalInvocationID.xy);
	int size = imageSize(texture).x;

	int i = 0;
	float x = float(size) / 2.0 * cos(2.0 * 3.1415 / i_Pulsation * i_Time);
	float y = float(size) / 2.0 * sin(2.0 * 3.1415 / i_Pulsation * i_Time);

	float smooth_x = smoothstep(x - 1.0, x, coords.x);
	float smooth_y = smoothstep(y - 1.0, y, coords.y);*/

	vec3 color = i_Color.xyz;

	imageStore(texture, ivec2(gl_GlobalInvocationID.xy), vec4(color, 1.0));
}