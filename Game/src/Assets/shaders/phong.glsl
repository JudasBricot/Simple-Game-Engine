#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform sampler2D u_HeightMap;
uniform sampler2D u_NormalMap;

out vec3 normal

void main()
{
	v_TexCoords	= a_TexCoords;

	vec3 position = vec3(a_Position.x, texture(u_HeightMap, a_TexCoords).x, a_Position.z);
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
}

#type fragment
#version 330 core
