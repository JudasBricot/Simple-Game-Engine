#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t Getheight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
	private:
		void loadTexture(const unsigned char* data, int width, int height, GLenum internalFormat, GLenum dataFormat) {}
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
	};
}
