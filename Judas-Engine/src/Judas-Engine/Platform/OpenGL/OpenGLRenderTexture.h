#pragma once

#include "Judas-Engine/Renderer/Texture.h"

namespace Judas_Engine
{
	class OpenGLRenderTexture2D : public RenderTexture2D
	{
	public:
		OpenGLRenderTexture2D(unsigned int width, unsigned int height);
		~OpenGLRenderTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
	private:
		unsigned int m_Width, m_Height;
		unsigned int m_RenderID;
	};
}
