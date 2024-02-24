#include "jepch.h"
#include "Renderer2D.h"

#include "Core/Core.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "glm/ext.hpp"

namespace Judas_Engine
{
	struct Renderer2DStorage
	{
		Ref<Shader> FlatColorShader;
		Ref<Shader> TextureShader;
		Ref<VertexArray> VertexArray;
	};

	static Scope<Renderer2DStorage> s_Data = CreateScope<Renderer2DStorage>();

	Renderer2D::Renderer2D() { }

	Renderer2D::~Renderer2D()
	{
		s_Data.release();
	}

	void Renderer2D::Init()
	{
		RenderCommand::Init();

		s_Data->VertexArray = VertexArray::Create();

		float vertices[4 * 5] = {
			-0.75f, -0.75f, 0.0f, 0.0f, 0.0f,
			 0.75f, -0.75f, 0.0f, 1.0f, 0.0f,
			 0.75f,  0.75f, 0.0f, 1.0f, 1.0f,
			-0.75f,  0.75f, 0.0f, 0.0f, 1.0f
		};
		Ref<VertexBuffer> m_VertexBuffer;
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position"},
			{ ShaderDataType::Float2, "a_TexCoords"},
		};
		m_VertexBuffer->SetLayout(layout);

		s_Data->VertexArray->AddVertexBuffer(m_VertexBuffer);

		unsigned int indices[6] = {
			0, 1, 2,
			0, 2, 3
		};

		Ref<IndexBuffer> m_IndexBuffer;
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		s_Data->VertexArray->SetIndexBuffer(m_IndexBuffer);

		s_Data->FlatColorShader = Shader::Create("src/Assets/Shaders/flatColor.glsl");
		s_Data->TextureShader = Shader::Create("src/Assets/Shaders/texture.glsl");
	}

	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer2D::BeginScene(OrthographicCamera& camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());		
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene() { }

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->SetFloat4("u_Color", color);

		s_Data->VertexArray->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0 });
		s_Data->FlatColorShader->SetMat4("u_Transform", transform);

		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture)
	{
		s_Data->TextureShader->Bind();
		s_Data->VertexArray->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0), position) * glm::scale(glm::mat4(1.0), { size.x, size.y, 1.0 });
		s_Data->TextureShader->SetMat4("u_Transform", transform);
		
		texture->Bind(0);

		RenderCommand::DrawIndexed(s_Data->VertexArray);
	}
}