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

out vec3 f_Position;
out vec3 f_Normal;
out vec3 f_Color;
out vec2 f_TexCoords;

void main()
{
	vec3 position = a_Position;
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);

	f_Position = gl_Position.xyz; // We have to rotate the normals ! mat3 extracts the rotation matrix from the transform
	f_Normal = mat3(u_Transform) * a_Normal;
	f_TexCoords	= a_TexCoords;
	f_Color = a_Color;

}

#type fragment
#version 330 core

struct Light
{
	vec3 Direction;

	vec3 AmbientColor;
	vec3 DiffuseColor;
	vec3 SpecularColor;
};

struct Material
{
	vec3 DiffuseColor;
	float DiffuseReflection;

	vec3 SpecularColor;
	float SpecularReflection;
	float SpecularShininess;
};

uniform Light u_Light;
uniform Material u_Material;
uniform vec3 u_CameraPosition;

in vec3 f_Position;
in vec3 f_Normal;
in vec3 f_Color;
in vec2 f_TexCoords;

layout(location = 0) out vec4 color;

void main()
{
	vec3 l = normalize(-u_Light.Direction);
	vec3 n = normalize(f_Normal);

	vec3 ambient = u_Light.AmbientColor;

	vec3 diffuse = u_Material.DiffuseReflection * max(dot(l, n), 0.0) * u_Material.DiffuseColor * u_Light.DiffuseColor;

	vec3 r = 2.0 * dot(n, l) * n - l;
	vec3 v = normalize(u_CameraPosition - f_Position);
	vec3 specular = u_Material.SpecularReflection * pow(max(dot(v, r), 0), u_Material.SpecularShininess) * u_Material.SpecularColor * u_Light.SpecularColor;

	color = vec4(ambient + diffuse + specular, 1.0);
}
