#pragma once

#include "Buffers/VertexBuffer.h"
#include "Buffers/VertexBufferLayout.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);
	void Bind();
	void Unbind();
private:
	unsigned int m_RendererID;
};

