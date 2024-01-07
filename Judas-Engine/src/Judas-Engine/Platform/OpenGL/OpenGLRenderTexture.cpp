#include "jepch.h"
#include "OpenGLRenderTexture.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	OpenGLRenderTexture2D::OpenGLRenderTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height), m_Slot(0)
	{
		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glActiveTexture(GL_TEXTURE0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	OpenGLRenderTexture2D::~OpenGLRenderTexture2D()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGLRenderTexture2D::Bind(uint32_t slot)
	{
		// ZOFNFIUZOF
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glBindImageTexture(slot, m_RenderID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	}

	void OpenGLRenderTexture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindImageTexture(m_Slot, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
	}

	void OpenGLRenderTexture2D::BindImage(uint32_t slot)
	{
		m_Slot = slot;
		glBindImageTexture(slot, m_RenderID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}

	void OpenGLRenderTexture2D::UnbindImage() const
	{
		glBindImageTexture(m_Slot, 0, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}
}
