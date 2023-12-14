#pragma once

#include "glm/glm.hpp"

namespace Judas_Engine
{
	class OpenGLSsbo
	{
	public:
		OpenGLSsbo(Ref<glm::vec3> data, unsigned int size);
		~OpenGLSsbo() {}

		void Bind(unsigned int index) const;
		void UnBind() const;

		void SendData() const;

	private:
		unsigned int m_RendererID;
		Ref<glm::vec3> m_Data;
		unsigned int m_Size;
	};
}