#include <FedPCH.h>
#include "Mesh.h"

namespace Fed
{
	// Lazy shell constructor. Must use SetBufferData before use
	Mesh::Mesh()
	{
	}
	Mesh::Mesh(const void* vertexData, unsigned int vertexBufferSize,
		const unsigned int* indexData, unsigned int indexSize, Texture texture)
		:	m_VertexBuffer(vertexData, vertexBufferSize),
			m_IndexBuffer(indexData, indexSize),
			m_Texture(texture)
	{
		m_Texture.LoadTexture();
		SetBufferLayout(GetBufferLayout());
		m_VertexArray.AddBuffer(m_VertexBuffer, m_BufferLayout);
	}

	Mesh::Mesh(const Mesh & other)
		:	m_VertexBuffer(other.m_VertexBuffer),
			m_IndexBuffer(other.m_IndexBuffer),
			m_Texture(other.m_Texture)
	{
		ASSERT(false, "Cannot copy over Base Mesh Object");
	}

	Mesh::~Mesh()
	{
	}
	void Mesh::Draw(Shader & shader)
	{
		if (m_VertexBuffer.IsEmpty() || m_IndexBuffer.IsEmpty() || m_Texture.GetWidth() == 0)
		{
			ASSERT(false, "Mesh cannot draw. It is empty");
		}
		m_Texture.Bind();
		shader.SetUniform1i("u_Texture", 0);
		Renderer renderer;
		renderer.Draw(m_VertexArray, m_IndexBuffer, shader);
	}
	void Mesh::SetData(const void * vertexData, unsigned int vertexBufferSize, const unsigned int * indexData, unsigned int indexSize, const Texture& texture)
	{
		m_VertexBuffer.SetBufferData(vertexData, vertexBufferSize);
		m_IndexBuffer.SetBufferData(indexData, indexSize);
		m_Texture.LoadTexture(texture.GetFilePath());
		m_VertexArray.AddBuffer(m_VertexBuffer, m_BufferLayout);
	}
	// Standard layout of vertex data. Meant to be overriden in child classes
	VertexBufferLayout Mesh::GetBufferLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(2); // Texture Coords
		return layout;
	}

	// Private Methods
	void Mesh::SetBufferLayout(VertexBufferLayout layout)
	{
		m_BufferLayout = layout;
	}
}

