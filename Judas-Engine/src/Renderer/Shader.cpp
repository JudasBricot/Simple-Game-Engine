#include "jepch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Judas_Engine
{
	Shader* Shader::Create(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::OpenGL:	return new OpenGLShader(filepath);
			case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::OpenGL:	return new OpenGLShader(vertexSrc, fragmentSrc);
		case RendererAPI::API::None:		JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}
