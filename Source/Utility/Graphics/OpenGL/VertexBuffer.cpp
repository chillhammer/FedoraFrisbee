#include <FedPCH.h>
#include "VertexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Fed
{
	VertexBuffer::VertexBuffer() : m_Data(nullptr), m_Size(0)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
	}
	VertexBuffer::VertexBuffer(const void * data, unsigned int size)
		:	m_Data(data), m_Size(size)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		SetBufferData(m_Data, m_Size);
	}

	VertexBuffer::VertexBuffer(const VertexBuffer & other)
		:	m_Data(other.m_Data), m_Size(other.m_Size)
	{
		ASSERT(true, "Restriction on VertexBuffer Copy Constructor");
		GLCall(glGenBuffers(1, &m_RendererID));
		SetBufferData(m_Data, m_Size);
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void VertexBuffer::SetBufferData(const void * data, unsigned int size)
	{
		if (size == 0)
		{
			__debugbreak();
		}
		m_Data = data;
		m_Size = size;
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Size, m_Data, GL_STATIC_DRAW));
	}

	bool VertexBuffer::IsEmpty() const
	{
		return m_Size == 0;
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}