#version 430 core
#define M_PI 3.1415926535897932384626433832795

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba32f) readonly uniform image2D butterflyTexture;
layout(binding = 1, rgba32f) uniform image2D pingpong0;
layout(binding = 2, rgba32f) uniform image2D pingpong1;

layout(std430, binding = 3) buffer indices {
    int stage;
    int pingpong;
    int direction;
};

vec2 mul(vec2 c0, vec2 c1) {
    vec2 c;
    c.x = c0.x * c1.x - c0.y * c1.y;
    c.y = c0.x * c1.y + c0.y * c1.x;
    return c;
}

void horizontalButterflies() {
    vec2 H;
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);
    vec4 data = imageLoad(butterflyTexture, ivec2(x.x, stage)).rgba;

    if (pingpong == 0) {
        vec2 p = imageLoad(pingpong0, ivec2(data.z, x.y)).rg;
        vec2 q = imageLoad(pingpong0, ivec2(data.w, x.y)).rg;
        vec2 w = vec2(data.x, data.y);

        H = p + mul(w, q);
        imageStore(pingpong1, x, vec4(H.x, H.y, 0, 1));
    } else if (pingpong == 1) {
        vec2 p = imageLoad(pingpong1, ivec2(data.z, x.y)).rg;
        vec2 q = imageLoad(pingpong1, ivec2(data.w, x.y)).rg;
        vec2 w = vec2(data.x, data.y);

        H = p + mul(w, q);
        imageStore(pingpong0, x, vec4(H.x, H.y, 0, 1));
    }
}

void verticalButterflies() {
    vec2 H;
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);
    vec4 data = imageLoad(butterflyTexture, ivec2(x.y, stage)).rgba;

    if (pingpong == 0) {
        vec2 p = imageLoad(pingpong0, ivec2(x.x, data.z)).rg;
        vec2 q = imageLoad(pingpong0, ivec2(x.x, data.w)).rg;
        vec2 w = vec2(data.x, data.y);

        H = p + mul(w, q);
        imageStore(pingpong1, x, vec4(H.x, H.y, 0, 1));
    } else if (pingpong == 1) {
        vec2 p = imageLoad(pingpong1, ivec2(x.x, data.z)).rg;
        vec2 q = imageLoad(pingpong1, ivec2(x.x, data.w)).rg;
        vec2 w = vec2(data.x, data.y);

        H = p + mul(w, q);
        imageStore(pingpong0, x, vec4(H.x, H.y, 0, 1));
    }
}

void main(void) {
    if (direction == 0)
        horizontalButterflies();
    else if (direction == 1)
        verticalButterflies();
}