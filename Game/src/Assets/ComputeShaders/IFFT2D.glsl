#version 430 core
#define M_PI 3.1415926535897932384626433832795

layout(local_size_x = 16, local_size_y = 16) in;
layout(binding = 0, rgba32f) readonly uniform image2D butterflyTexture;
layout(binding = 1, rgba32f) uniform image2D pingpong0;
layout(binding = 2, rgba32f) uniform image2D pingpong1;

struct complex {
    float real;
    float im;
};

complex mul(complex c0, complex c1) {
    complex c;
    c.real = c0.real * c1.real - c0.im * c1.im;
    c.im = c0.real * c1.im + c0.im * c1.real;
    return c;
}

complex add(complex c0, complex c1) {
    complex c;
    c.real = c0.real + c1.real;
    c.im = c0.im + c1.im;
    return c;
}

void horizontalButterflies(int stage, int pingpong, int direction) {
    complex H;
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);

    if (pingpong == 0) {
        vec4 data = imageLoad(butterflyTexture, ivec2(x.x, stage)).rgba;
        vec2 p_ = imageLoad(pingpong0, ivec2(data.z, x.y)).rg;
        vec2 q_ = imageLoad(pingpong0, ivec2(data.w, x.y)).rg;
        vec2 w_ = vec2(data.x, data.y);

        complex p = complex(p_.x, p_.y);
        complex q = complex(q_.x, q_.y);
        complex w = complex(w_.x, w_.y);

        H = add(p, mul(w, q));
        imageStore(pingpong1, x, vec4(H.real, H.im, 0, 1));
    } else if (pingpong == 1) {
        vec4 data = imageLoad(butterflyTexture, ivec2(x.x, stage)).rgba;
        vec2 p_ = imageLoad(pingpong1, ivec2(data.z, x.y)).rg;
        vec2 q_ = imageLoad(pingpong1, ivec2(data.w, x.y)).rg;
        vec2 w_ = vec2(data.x, data.y);

        complex p = complex(p_.x, p_.y);
        complex q = complex(q_.x, q_.y);
        complex w = complex(w_.x, w_.y);

        H = add(p, mul(w, q));
        imageStore(pingpong0, x, vec4(H.real, H.im, 0, 1));
    }
}

void verticalButterflies(int stage, int pingpong, int direction) {
    complex H;
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);

    if (pingpong == 0) {
        vec4 data = imageLoad(butterflyTexture, ivec2(x.y, stage)).rgba;
        vec2 p_ = imageLoad(pingpong0, ivec2(x.x, data.z)).rg;
        vec2 q_ = imageLoad(pingpong0, ivec2(x.x, data.w)).rg;
        vec2 w_ = vec2(data.x, data.y);

        complex p = complex(p_.x, p_.y);
        complex q = complex(q_.x, q_.y);
        complex w = complex(w_.x, w_.y);

        H = add(p, mul(w, q));
        imageStore(pingpong1, x, vec4(H.real, H.im, 0, 1));
    } else if (pingpong == 1) {
        vec4 data = imageLoad(butterflyTexture, ivec2(x.y, stage)).rgba;
        vec2 p_ = imageLoad(pingpong1, ivec2(x.x, data.z)).rg;
        vec2 q_ = imageLoad(pingpong1, ivec2(x.x, data.w)).rg;
        vec2 w_ = vec2(data.x, data.y);

        complex p = complex(p_.x, p_.y);
        complex q = complex(q_.x, q_.y);
        complex w = complex(w_.x, w_.y);

        H = add(p, mul(w, q));
        imageStore(pingpong0, x, vec4(H.real, H.im, 0, 1));
    }
}

void pass(int stage, int pingpong, int direction) 
{
    if (direction == 0)
        horizontalButterflies(stage, pingpong, direction);
    else if (direction == 1)
        verticalButterflies(stage, pingpong, direction);
}

void main()
{
    for (int direction = 1; direction > -1; direction--)
		{
			for (int stage = 0; stage < 8; stage++)
			{
				pass(stage, stage % 2, direction);
			}
		}
}