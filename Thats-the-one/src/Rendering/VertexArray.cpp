#include "VertexArray.h"

#include "Renderer.h"
#include "Buffers/VertexBufferLayout.h"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout)
{
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (int i = 0; i < elements.size(); i++)
    {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
        offset += element.count * VertexBufferElement::GetSizeFromType(element.type);
    }
}

void VertexArray::Bind()
{
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
    GLCall(glBindVertexArray(0));
}
