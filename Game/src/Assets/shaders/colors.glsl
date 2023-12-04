#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec2 a_TexCoords;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform; // PB !!

out vec3 v_Color;
out vec3 v_Position;

void main()
{
	v_Color = a_Color;
	v_Position = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
			
in vec3 v_Position;	
in vec3 v_Color;		
layout(location = 0) out vec4 color;

void main()
{
	color = vec4(v_Color, 1.0);
}