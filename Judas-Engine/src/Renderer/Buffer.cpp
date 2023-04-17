#include "jepch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Judas_Engine
{
	VertexBuffer* Judas_Engine::VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
		case RendererAPI::None:
			JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");
			return nullptr;
		default:
			JE_CORE_ASSERT(false, "Unknown Render API");
			return nullptr;
		}
	}
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
		case RendererAPI::None:
			JE_CORE_ASSERT(false, "RendererAPI::None is not currently supported");
			return nullptr;
		default:
			JE_CORE_ASSERT(false, "Unknown Render API");
			return nullptr;
		}
	}
}
