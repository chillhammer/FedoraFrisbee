#include <FedPCH.h>
#include "IndexBuffer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Fed
{
	IndexBuffer::IndexBuffer()
	{
		GLCall(glGenBuffers(1, &m_RendererID));
	}
	IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) :
		m_Data(data), m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		SetBufferData(m_Data, m_Count);
	}

	IndexBuffer::IndexBuffer(const IndexBuffer & other)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		SetBufferData(other.m_Data, other.m_Count);
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void IndexBuffer::SetBufferData(const unsigned int * data, unsigned int count)
	{
		m_Data = data;
		m_Count = count;
		Bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned int), m_Data, GL_STATIC_DRAW));
	}

	bool IndexBuffer::IsEmpty()
	{
		return m_Count == 0;
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	}

	void IndexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}