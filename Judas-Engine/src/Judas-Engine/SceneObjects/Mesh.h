#pragma once

#include "Judas-Engine/Renderer/Renderer.h"
#include "Judas-Engine/Renderer/Shader.h"
#include "Judas-Engine/Renderer/VertexArray.h"
#include "Judas-Engine/Renderer/Buffer.h"
#include "Judas-Engine/Core/Core.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

namespace Judas_Engine
{
	class Mesh
	{
	public:
		Mesh() {}
		Mesh(const Ref<Shader> shader, const Ref<VertexArray> vertexArray, glm::vec3 scale) : m_Shader(shader), m_VertexArray(vertexArray), m_Scale(scale) {}
		~Mesh() {}

		void SetMeshData(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, const BufferLayout& layout);
		void SetShader(const Ref<Shader> shader) { m_Shader = shader; }

		const void Submit();
	private:
		Ref<Shader> m_Shader{};
		Ref<VertexArray> m_VertexArray{};
		glm::vec3 m_Scale{};

		Ref<VertexBuffer> m_VertexBuffer{};
		Ref<IndexBuffer> m_IndexBuffer{};

		std::vector<float> m_Vertices{};
		std::vector<unsigned int> m_Indices{};
	};
}