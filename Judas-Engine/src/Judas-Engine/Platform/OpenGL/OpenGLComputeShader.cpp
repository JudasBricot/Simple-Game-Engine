#include "jepch.h"
#include "OpenGLComputeShader.h"

#include <glad/glad.h>

namespace Judas_Engine
{
	OpenGLComputeShader::OpenGLComputeShader(const std::string& filepath)
	{
	}

	OpenGLComputeShader::OpenGLComputeShader(const std::string& name, const std::string& src) : m_Name(name)
	{
		Compile(src);
	}

	void OpenGLComputeShader::InitTexture()
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glActiveTexture(GL_TEXTURE0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA,
			GL_FLOAT, NULL);

		glBindImageTexture(0, m_Texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
	}

	void OpenGLComputeShader::BindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);
	}

	void OpenGLComputeShader::Dispatch() const
	{
		glUseProgram(m_RendererID);
		glBindImageTexture(0, m_Texture, 0, GL_FALSE,0, GL_WRITE_ONLY, GL_RGBA32F);
		glDispatchCompute((unsigned int)TEXTURE_WIDTH, (unsigned int)TEXTURE_HEIGHT, 1);

		//glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void OpenGLComputeShader::Compile(const std::string& src)
	{
		unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);

		const GLchar* source = (const GLchar*)src.c_str();
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(shader);

			// Use the infoLog as you see fit.
			JE_CORE_ERROR("{0}", infoLog.data());
			JE_CORE_ASSERT(false, "Shader compilation failure");

			// In this simple program, we'll just leave
			return;
		}

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, shader);
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(shader);

			// Use the infoLog as you see fit.
			JE_CORE_ERROR("{0}", infoLog.data());
			JE_CORE_ASSERT(false, "Shader linking failed");

			// In this simple program, we'll just leave
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, shader);
	}
}
