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
uniform sampler2D u_DisplacementMap;

uniform float u_DisplacementFactor;

out vec3 f_Position;
out vec3 f_Normal;
out vec3 f_Color;
out vec2 f_TexCoords;

void main()
{
	vec2 normal_2D = texture(u_NormalMap, a_TexCoords).xy;
	vec2 displacement_2D = u_DisplacementFactor * texture(u_DisplacementMap, a_TexCoords).xy;

	vec3 position = vec3(a_Position.x + displacement_2D.x, texture(u_HeightMap, a_TexCoords).x, a_Position.z + displacement_2D.y);

	f_Position = (u_Transform * vec4(position, 1.0)).xyz;
	f_Normal = inverse(transpose(mat3(u_Transform))) * vec3(-normal_2D.x, 1.0, -normal_2D.y);
	f_TexCoords	= a_TexCoords;
	f_Color = a_Color;

	gl_Position = u_ViewProjection * vec4(f_Position, 1.0);
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
	diffuse = clamp(diffuse, 0.0, 1.0);

	vec3 r = 2.0 * dot(n, l) * n - l;
	vec3 v = normalize(u_CameraPosition - f_Position);
	vec3 specular = u_Material.SpecularReflection * pow(max(dot(v, r), 0), u_Material.SpecularShininess) * u_Material.SpecularColor * u_Light.SpecularColor;
	specular = clamp(specular, 0.0, 1.0);

	color = vec4(ambient + diffuse + specular, 1.0);
}
