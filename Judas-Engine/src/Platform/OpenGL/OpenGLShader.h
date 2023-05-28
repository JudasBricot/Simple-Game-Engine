#pragma once

#include "Renderer/Shader.h"

#include <glm/glm.hpp>

namespace Judas_Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		void Bind() const;
		void Unbind() const;


		void UploadUniformInt(const char* name, int value);

		void UploadUniformFloat(const char* name, float value);
		void UploadUniformFloat2(const char* name, const glm::vec2 values);
		void UploadUniformFloat3(const char* name, const glm::vec3 values);
		void UploadUniformFloat4(const char* name, const glm::vec4 values);

		void UploadUniformMat3(const char* name, const glm::mat3 mat);
		void UploadUniformMat4(const char* name, const glm::mat4 mat);
	private:
		unsigned int m_RendererID;
	};
}

