#include <FedPCH.h>
#include "Mesh.h"

namespace Fed
{
	Mesh::Mesh(const void* vertexData, unsigned int vertexBufferSize, 
		const unsigned int* indexData, unsigned int indexSize, Texture& texture)
		:	m_VertexBuffer(vertexData, vertexBufferSize),
			m_IndexBuffer(indexData, indexSize),
			m_Texture(texture)
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

	const Texture & Mesh::GetTexture() const
	{
		return m_Texture;
	}

	// Private Methods
	void Mesh::SetBufferLayout(VertexBufferLayout layout)
	{
		m_BufferLayout = layout;
	}

	// Debug function to draw mesh
	void Msh::DrawMesh(const Mesh & mesh, Shader & shader)
	{
		mesh.GetTexture().Bind();
		shader.SetUniform1i("u_Texture", 0);
		Renderer renderer;
		renderer.Draw(mesh.GetVertexArray(), mesh.GetIndexBuffer(), shader);
	}
}

