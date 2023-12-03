#include "jepch.h"
#include "Mesh.h"

namespace Judas_Engine
{
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