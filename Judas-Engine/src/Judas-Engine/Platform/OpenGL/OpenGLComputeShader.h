#pragma once

#include "Judas-Engine/Compute/ComputeShader.h"

typedef unsigned int GLenum;

namespace Judas_Engine
{
	class OpenGLComputeShader : public ComputeShader
	{
	public:
		OpenGLComputeShader(const std::string& filepath);
		OpenGLComputeShader(const std::string& name, const std::string& src);

		virtual const std::string GetName() override { return m_Name; }

		void InitTexture();
		void BindTexture();

		virtual void Dispatch() const override;

	private:
		void Compile(const std::string& src);

	private:
		std::string m_Name;
		unsigned int m_RendererID;
		unsigned int m_Texture;
		unsigned int m_Fbo;

		const unsigned int TEXTURE_WIDTH = 512, TEXTURE_HEIGHT = 512;	
	};
}