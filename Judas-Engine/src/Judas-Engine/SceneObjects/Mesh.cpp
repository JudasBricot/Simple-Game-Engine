#include "jepch.h"
#include "Mesh.h"

namespace Judas_Engine
{
	const Ref<Mesh> Mesh::CreatePrimitive(PrimitiveType type, unsigned int resolution)
	{
		std::vector<float> vertices;
		BufferLayout layout;
		std::vector<unsigned int> indices;

		Ref<Shader> shader;

		switch (type)
		{
			case Judas_Engine::Cube:
			{
				vertices = {
					-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
					 0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
					-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
					 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f,

					-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
					-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,

					-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
					 0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
					-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
					 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,

					 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
					 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
					 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
					 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,

					-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
					-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f,

					-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
					-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f
				};

				indices = {
					0, 1, 2,
					1, 3, 2,

					4, 5, 6,
					5, 7, 6,

					8, 10, 9,
					9, 10, 11,

					12, 14, 13,
					13, 14, 15,

					16, 18, 17,
					17, 18, 19,

					20, 21, 22,
					21, 23, 22
				};
				break;
			}
			case Judas_Engine::Plane:
			{
				vertices = {
					-0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 1.0f,
					-0.5f, 0.0f,  0.5f, 1.0f, 0.0f, 1.0f,
					 0.5f, 0.0f,  0.5f, 1.0f, 0.0f, 1.0f
				};

				indices = {
					0, 2, 1,
					1, 2, 3,
				};
				break;
			}
			default:
				break;
		}

		layout = {
			{ Judas_Engine::ShaderDataType::Float3, "a_Position"},
			{ Judas_Engine::ShaderDataType::Float3, "a_Color"}
		};

		std::string vertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec3 a_Color;

				uniform mat4 u_ViewProjection;

				out vec3 v_Color;
				out vec3 v_Position;

				void main()
				{
					v_Color = a_Color;
					v_Position = a_Position;
					gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
				}
				)";

		std::string fragmentSrc = R"(
				#version 330 core
			
				in vec3 v_Position;	
				in vec3 v_Color;		
				layout(location = 0) out vec4 color;

				void main()
				{
					color = vec4(v_Color, 1.0);
				}
				)";

		shader = Shader::Create("Shader", vertexSrc, fragmentSrc);

		Ref<Mesh> mesh = std::make_shared<Mesh>();
		mesh->SetShader(shader);
		mesh->SetMeshData(vertices, indices, layout);

		return mesh;
	}

	void Mesh::SetMeshData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const BufferLayout& layout)
	{
		m_Vertices = vertices;
		m_VertexBuffer.reset(Judas_Engine::VertexBuffer::Create(m_Vertices.data(), sizeof(float) * m_Vertices.size()));
		m_VertexBuffer->SetLayout(layout);

		m_Indices = indices;
		m_IndexBuffer.reset(Judas_Engine::IndexBuffer::Create(m_Indices.data(), m_Indices.size()));

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}
	const void Mesh::Submit()
	{
		JE_ASSERT(m_Shader, "Mesh Shader is null"); 
		JE_ASSERT(m_VertexArray, "Mesh Vertex array is null");
		JE_ASSERT(m_VertexBuffer, "Mesh Vertex buffer is null");
		JE_ASSERT(m_IndexBuffer, "Mesh Index bufferis null");

		Renderer::Submit(m_Shader, m_VertexArray, glm::scale(glm::mat4x4(1.0), m_Scale)); 
	}
}