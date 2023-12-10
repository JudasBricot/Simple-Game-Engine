#include "jepch.h"
#include "OpenGLRenderTexture.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	OpenGLRenderTexture2D::OpenGLRenderTexture2D(unsigned int width, unsigned int height) : m_Width(width), m_Height(height)
	{
		glGenTextures(1, &m_RenderID);
		glBindTexture(GL_TEXTURE_2D, m_RenderID);
		glActiveTexture(GL_TEXTURE0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, NULL);

		glBindImageTexture(0, m_RenderID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}

	OpenGLRenderTexture2D::~OpenGLRenderTexture2D()
	{
		glDeleteTextures(1, &m_RenderID);
	}

	void OpenGLRenderTexture2D::Bind(uint32_t slot) const
	{
		glBindImageTexture(slot, m_RenderID, 0, GL_FALSE,0, GL_WRITE_ONLY, GL_RGBA32F);
	}
}
