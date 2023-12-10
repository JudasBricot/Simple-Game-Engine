#include "jepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLRenderTexture.h"

namespace Judas_Engine
{
	Ref<RenderTexture2D> RenderTexture2D::Create(unsigned int width, unsigned int height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLRenderTexture2D>(width, height);
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const unsigned char* data, int width, int height, int channels)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(data, width, height, channels);
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:	JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");  return nullptr;
		case RendererAPI::API::OpenGL:	return std::make_shared<OpenGLTexture2D>(path);
		}

		JE_CORE_ASSERT(false, "Unknown Render API");
		return nullptr;
	}
}