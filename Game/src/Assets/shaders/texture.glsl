#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoords;

void main()
{
	v_TexCoords	= a_TexCoords;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

uniform sampler2D u_Texture;

in vec2 v_TexCoords;
					
layout(location = 0) out vec4 color;

void main()
{
	//color = vec4(v_TexCoords, 0.0, 1.0);
	color = texture(u_Texture, v_TexCoords);
}