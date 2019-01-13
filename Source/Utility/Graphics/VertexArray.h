#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Fed
{
	/**
		OpenGL wrapper to contain multiple vertex buffers.
		Uses BufferLayouts to assign them to buffer.
	**/
	class VertexArray
	{
	private:
		unsigned int m_RendererID;
	public:
		VertexArray();
		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	};
}