#include <FedPCH.h>
#include "Mesh.h"

namespace Fed
{
	Mesh::Mesh(const void* vertexData, unsigned int vertexBufferSize, 
		const unsigned int* indexData, unsigned int indexSize)
		:	m_VertexBuffer(vertexData, vertexBufferSize),
			m_IndexBuffer(indexData, indexSize)
	{
		SetBufferLayout(GetBufferLayout());
		m_VertexArray.AddBuffer(m_VertexBuffer, m_BufferLayout);
	}
	Mesh::~Mesh()
	{
	}
	// Standard layout of vertex data. Meant to be overriden in child classes
	VertexBufferLayout Mesh::GetBufferLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(3); // Position
		layout.Push<float>(2); // Texture Coords
		return layout;
	}

	// Public Getters
	const VertexArray & Mesh::GetVertexArray() const
	{
		return m_VertexArray;
	}
	const IndexBuffer & Mesh::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

	// Private Methods
	void Mesh::SetBufferLayout(VertexBufferLayout layout)
	{
		m_BufferLayout = layout;
	}

	// Debug function to draw mesh
	void Msh::DrawMesh(const Mesh & mesh, const Shader & shader)
	{
		Renderer renderer;
		renderer.Draw(mesh.GetVertexArray(), mesh.GetIndexBuffer(), shader);
	}
}

