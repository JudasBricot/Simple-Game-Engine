#include "jepch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path), m_Width(0), m_Height(0), m_Channels(0), m_RendererID(0)
	{
		SetTexture(path);
	}

	OpenGLTexture2D::OpenGLTexture2D(const unsigned char* data, int width, int height, int channels)
	{
		SetTexture(data, width, height, channels);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		m_Slot = slot;
		glBindTextureUnit(slot, m_RendererID);
	}

	// A reflechir : ajouter une securite ? unbind => on reset m_Slot et on ajoute un assert pour unbind pour eviter d'unbind plusieurs fois ?
	void OpenGLTexture2D::Unbind() const
	{
		glBindTextureUnit(m_Slot, 0);
	}

	void OpenGLTexture2D::SetTexture(const std::string& path)
	{
		int width, height, channels;

		stbi_set_flip_vertically_on_load(1);

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		JE_CORE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		m_RendererID = LoadTexture(data, width, height, channels);
		stbi_image_free(data);
	}

	void OpenGLTexture2D::SetTexture(const unsigned char* data, int width, int height, int channels)
	{
		// TO DO REMEMBER TEXTURE DATA ?
		JE_CORE_ASSERT(data, "The texture data pointer is NULL");
		JE_CORE_ASSERT(width, "The texture width cannot be 0");
		JE_CORE_ASSERT(height, "The texture height cannot be 0");
		JE_CORE_ASSERT(channels, "The texture channels cannot be 0");

		m_Width = width;
		m_Height = height;
		m_Channels = channels;

		m_RendererID = LoadTexture(data, width, height, channels);
	}

	unsigned int OpenGLTexture2D::LoadTexture(const unsigned char* data, int width, int height, int channels)
	{
		unsigned int rendererId;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		JE_CORE_ASSERT(internalFormat & dataFormat, "Data format not supported, must be 3 or 4 channels")

		glCreateTextures(GL_TEXTURE_2D, 1, &rendererId);
		glTextureStorage2D(rendererId, 1, internalFormat, width, height);

		glTextureParameteri(rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(rendererId, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		return rendererId;
	}
}