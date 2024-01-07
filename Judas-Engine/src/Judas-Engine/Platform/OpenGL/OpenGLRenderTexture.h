#pragma once

#include "Judas-Engine/Renderer/Texture.h"

namespace Judas_Engine
{
	class OpenGLRenderTexture2D : public RenderTexture2D
	{
	public:
		OpenGLRenderTexture2D(uint32_t width, uint32_t height);
		~OpenGLRenderTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind() const override;

		virtual void BindImage(uint32_t slot) override;
		virtual void UnbindImage() const override;
	private:
		unsigned int m_Slot, m_Unit;
		unsigned int m_Width, m_Height;
		unsigned int m_RenderID;
	};
}
