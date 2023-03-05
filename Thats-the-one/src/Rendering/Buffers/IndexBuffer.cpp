#include "IndexBuffer.h"

#include <GL/glew.h>
#include "../Renderer.h"

IndexBuffer::IndexBuffer(const void* data, const unsigned int count)
{
    GLCall(glGenBuffers(1, &m_RenderID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &m_RenderID));
}

void IndexBuffer::Bind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::Unbind()
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}
