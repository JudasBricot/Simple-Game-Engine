#include "jepch.h"
#include "ComputeShader.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLComputeShader.h"

namespace Judas_Engine
{
	Ref<ComputeShader> Judas_Engine::ComputeShader::Create(const std::string& filepath, Ref<RenderTexture2D> renderTexture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLComputeShader>(filepath, renderTexture);
		case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Ref<ComputeShader> Judas_Engine::ComputeShader::Create(const std::string& name, const std::string& src, Ref<RenderTexture2D> renderTexture)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLComputeShader>(name, src, renderTexture);
		case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
