#include "jepch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"

// Temp
#include <glm/gtx/string_cast.hpp>

namespace Judas_Engine
{
	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewPort(0, 0, width, height);
	}

	void Renderer::EnableBackFaceCulling()
	{
		RenderCommand::EnableBackFaceCulling();
	}

	void Renderer::EnableDepthTest()
	{
		RenderCommand::EnableDepthTest();
	}

	void Renderer::BeginScene(PerspectiveCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
		s_SceneData->CameraPosition = camera.GetPosition();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4 modelMatrix)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformFloat3("u_CameraPosition", s_SceneData->CameraPosition);

		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", modelMatrix);

		JE_INFO("Model matrix : \n {0}", glm::to_string(modelMatrix));

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}