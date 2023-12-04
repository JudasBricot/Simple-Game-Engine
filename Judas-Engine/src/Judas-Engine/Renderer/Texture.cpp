#include "jepch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Judas_Engine
{
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