#pragma once

class IndexBuffer
{
public:
	IndexBuffer(const void* data, const unsigned int count);
	~IndexBuffer();

	void Bind();
	void Unbind();
private:
	unsigned int m_RenderID;
};

