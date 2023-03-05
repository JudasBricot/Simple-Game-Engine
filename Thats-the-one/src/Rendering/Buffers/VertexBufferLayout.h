#pragma once

#include <GL/glew.h>
#include "../Renderer.h"

#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeFromType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}

		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeFromType(GL_FLOAT);
	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeFromType(GL_UNSIGNED_INT) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() { return m_Elements; }
	inline unsigned int GetStride() { return m_Stride; }

private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
};

