#pragma once

#include "Renderer/Texture.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		OpenGLTexture2D(const unsigned char* data, int width, int height, int channels);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t Getheight() const override { return m_Height; }

		virtual void Bind(uint32_t slot) const override;
		// TEMP
		void SetTexture(const std::string& path);
		void SetTexture(const unsigned char* data, int width, int height, int channels);
	private:
		// TEMP
		static unsigned int LoadTexture(const unsigned char* data, int width, int height, int channels);
	private:
		std::string m_Path;
		uint32_t m_Width, m_Height;
		// CHANGE DATA TYPE !!!
		uint32_t m_Channels;
		uint32_t m_RendererID;
	};
}
