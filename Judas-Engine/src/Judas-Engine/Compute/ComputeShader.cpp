#include "jepch.h"
#include "ComputeShader.h"

#include "Renderer/RendererAPI.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLComputeShader.h"

namespace Judas_Engine
{
	Ref<ComputeShader> Judas_Engine::ComputeShader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLComputeShader>(filepath);
		case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Ref<ComputeShader> Judas_Engine::ComputeShader::Create(const std::string& name, const std::string& src)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLComputeShader>(name, src);
		case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
