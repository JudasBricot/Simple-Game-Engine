#include "jepch.h"
#include "Mesh.h"

const float PI =  3.1415926535897932384626433832795;


namespace Judas_Engine
{
	const Ref<Mesh> Mesh::CreatePrimitive(PrimitiveType type, unsigned int resolution)
	{
		JE_CORE_ASSERT(resolution > 0, "Resolution cannot be zero");

		std::vector<float> vertices;
		BufferLayout layout;
		std::vector<unsigned int> indices;

		Ref<Shader> shader;

		switch (type)
		{
			case PrimitiveType::Cube:
			{
				// WRONG NORMALS !!! TEMPORARY
				vertices = {
					-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
					 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
					-0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					 0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

					-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
					-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
					-0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
					-0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

					-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
					 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
					-0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
					 0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,

					 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
					 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
					 0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
					 0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,

					-0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					 0.5f,  0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
					-0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
					 0.5f,  0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,

					-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
					 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
					-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
					 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f
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
			case PrimitiveType::Plane:
			{
				float delta = 1.0f / resolution;

				for (unsigned int i = 0; i < resolution + 1; i++)
				{
					for (unsigned int j = 0; j < resolution + 1; j++)
					{
						vertices.insert(vertices.end(), { 
							(float)i * delta - 0.5f, 0.0f,				(float)j * delta - 0.5f, // Position
							0.0f,					 1.0f,				0.0f,				     // Normal
							1.0f,					 0.0f,				1.0f,				     // Color
							(float)i * delta,		 (float)j * delta				             // Texture coordinates
						});
					}
				}

				for (unsigned int i = 0; i < resolution; i++)
				{
					for (unsigned int j = 0; j < resolution; j++)
					{
						unsigned int vertexID = i * (resolution + 1) + j;
						indices.insert(indices.end(), { 
							vertexID,				   vertexID + 1, vertexID + resolution + 1, 
							vertexID + resolution + 1, vertexID + 1, vertexID + resolution + 2
						});
					}
				}
				break;
			}
			case PrimitiveType::Sphere:
			{
				unsigned int sectors = 2U * (unsigned int)resolution;
				unsigned int rings = (unsigned int)resolution;

				const float S = 1.f / (float)(sectors - 1);
				const float R = 1.f / (float)(rings - 1);

				for (unsigned int s = 0; s < sectors; s++)
				{
					for (unsigned int r = 0; r < rings; r++)
					{
						float const theta = PI * (float)r * R;
						float const phi = 2.0f * PI * (float)s * S;

						float const y = cos(theta);
						float const x = sin(theta) * sin(phi);
						float const z = sin(theta) * cos(phi);

						unsigned int currSector = s;
						// Avoid index out of bounds
						unsigned int nextSector = ((s + 1) % sectors);

						unsigned int currRing = r;
						// Avoid index out of bounds
						unsigned int nextRing = ((r + 1) % rings);

						vertices.insert(vertices.end(), {
							 x,			  y,			       z, // Position
							 x, 		  y,				   z, // Normal
							 1.0f,		  0.0f,				1.0f, // Color
							 s * S,		  r * R,				  // Texture coordinates
							});

						indices.insert(indices.end(), {
							currSector * resolution + currRing, currSector * resolution + nextRing, nextSector * resolution + nextRing,
							nextSector * resolution + nextRing, nextSector * resolution + currRing, currSector * resolution + currRing
							});


						/*indices.push_back(currSector* resolution + currRing);
						indices.push_back(currSector * resolution + nextRing);
						indices.push_back(nextSector * resolution + nextRing);

						indices.push_back(nextSector * resolution + nextRing);
						indices.push_back(nextSector * resolution + currRing);
						indices.push_back(currSector * resolution + currRing);*/
					}
				}
				break;
			}
			default:
				break;
		}

		layout = {
			{ Judas_Engine::ShaderDataType::Float3, "a_Position"},
			{ Judas_Engine::ShaderDataType::Float3, "a_Normal"},
			{ Judas_Engine::ShaderDataType::Float3, "a_Color"},
			{ Judas_Engine::ShaderDataType::Float2, "a_TexCoords"}
		};

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec3 a_Normal;
			layout(location = 2) in vec3 a_Color;
			layout(location = 3) in vec2 a_TexCoords;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Color;
			out vec3 v_Position;

			void main()
			{
				v_Color = a_Color;
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
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
		m_VertexBuffer.reset(Judas_Engine::VertexBuffer::Create(m_Vertices.data(), sizeof(float) * (uint32_t)m_Vertices.size()));
		m_VertexBuffer->SetLayout(layout);

		m_Indices = indices;
		m_IndexBuffer.reset(Judas_Engine::IndexBuffer::Create(m_Indices.data(), (uint32_t)m_Indices.size()));

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	const void Mesh::Submit()
	{
		JE_ASSERT(m_Shader, "Mesh Shader is null"); 
		JE_ASSERT(m_VertexArray, "Mesh Vertex array is null");
		JE_ASSERT(m_VertexBuffer, "Mesh Vertex buffer is null");
		JE_ASSERT(m_IndexBuffer, "Mesh Index buffer is null");

		Renderer::Submit(m_Shader, m_VertexArray, m_Transform.GetModelMatrix()); 
	}
}