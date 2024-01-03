#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

uniform sampler2D u_Texture;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords	= a_TexCoords;

	vec3 position = a_Position;
	//vec3 position = vec3(a_Position.x, texture(u_Texture, a_TexCoords).x, a_Position.z);

	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
}

#type fragment
#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoords;
					
layout(location = 0) out vec4 color;

void main()
{
	//color = vec4(v_TexCoords, 0.0, 1.0);
	color = 15.0 * (texture(u_Texture, v_TexCoords) + 0.01);
}