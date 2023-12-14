#include "jepch.h"
#include "OpenGLSsbo.h"

#include "glm/ext.hpp"

#include <glad/glad.h>

namespace Judas_Engine
{ 

	OpenGLSsbo::OpenGLSsbo(Ref<glm::vec3> data, unsigned int size) : m_Data(data), m_Size(size)
	{
		glGenBuffers(1, &m_RendererID);
		SendData();
	}

	void OpenGLSsbo::Bind(unsigned int index) const
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, m_RendererID);
	}

	void OpenGLSsbo::UnBind() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	void OpenGLSsbo::SendData() const
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_RendererID);
		glBufferData(GL_SHADER_STORAGE_BUFFER, m_Size, m_Data.get(), GL_DYNAMIC_COPY);
	}
}