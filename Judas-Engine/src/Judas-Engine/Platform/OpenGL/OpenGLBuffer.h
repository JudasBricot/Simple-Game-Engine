#pragma once

#include "Judas-Engine/Renderer/Buffer.h"

#include "glm/glm.hpp"

namespace Judas_Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};

	class OpenGLDataBufferObject
	{
	public:
		OpenGLDataBufferObject(const Ref<void>& data, size_t size);
		~OpenGLDataBufferObject() {}

		void Bind(unsigned int index = 0) const;
		void UnBind() const;

		void UpdateData(const Ref<void>& data) const;

	private:
		unsigned int m_RendererID; 
		unsigned int m_Size;
	};

}
