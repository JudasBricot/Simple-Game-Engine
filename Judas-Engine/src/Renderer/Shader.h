#pragma once
#include <string>

namespace Judas_Engine
{
	class Shader
	{
	public:
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		void Bind() const;
		void Unbind() const;
	private:
		unsigned int m_RendererID;
	};
}
