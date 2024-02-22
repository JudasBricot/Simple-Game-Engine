#pragma once

#include "RenderCommand.h"
#include "Camera/PerspectiveCamera.h"
#include "Shader.h"

namespace Judas_Engine
{
	class Renderer
	{
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void EnableBackFaceCulling();
		static void EnableDepthTest();

		static void BeginScene(PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
			glm::vec3 CameraPosition;
		};

		static SceneData* s_SceneData;
	};
}
