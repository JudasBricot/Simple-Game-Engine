#include "jepch.h"
#include "OpenGLComputeShader.h"

#include <glad/glad.h>

#include <fstream>

#include <glm/ext.hpp>

namespace Judas_Engine
{
	OpenGLComputeShader::OpenGLComputeShader(const std::string& filepath, Ref<RenderTexture2D> renderTexture, uint32_t textureSlot) : m_RenderTexture(renderTexture), m_TextureSlot(textureSlot)
	{
		JE_CORE_ASSERT(m_RenderTexture, "Render texture is null");

		std::string source = ReadFile(filepath);
		Compile(source);

		// Find Name
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind(".");
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLComputeShader::OpenGLComputeShader(const std::string& name, const std::string& src, Ref<RenderTexture2D> renderTexture, uint32_t textureSlot) : m_Name(name), m_RenderTexture(renderTexture), m_TextureSlot(textureSlot)
	{
		JE_CORE_ASSERT(m_RenderTexture, "Render texture is null");
		Compile(src);
	}

	void OpenGLComputeShader::Dispatch() const
	{
		glUseProgram(m_RendererID);
		//m_RenderTexture->Bind(m_TextureSlot);

		glDispatchCompute(m_RenderTexture->GetWidth(), m_RenderTexture->GetHeight(), 1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	std::string OpenGLComputeShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);

		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			JE_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
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
