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

	Mesh::Mesh(const void* vertexData, unsigned int vertexBufferSize,
		const unsigned int* indexData, unsigned int indexSize, Vector3 color)
		: m_VertexBuffer(vertexData, vertexBufferSize),
		m_IndexBuffer(indexData, indexSize),
		m_Color(color)
	{
		SetBufferLayout(GetBufferLayout());
		m_VertexArray.AddBuffer(m_VertexBuffer, m_BufferLayout);
	}

	Mesh::Mesh(const Mesh & other)
		:	m_VertexBuffer(other.m_VertexBuffer),
			m_IndexBuffer(other.m_IndexBuffer),
			m_Texture(other.m_Texture),
			m_Color(other.m_Color)
	{
		ASSERT(false, "Cannot copy over Base Mesh Object");
	}

	Mesh::~Mesh()
	{
	}
	void Mesh::Draw(const ShaderPtr& shader, const Matrix4x4& model)
	{
		if (m_VertexBuffer.IsEmpty() || m_IndexBuffer.IsEmpty() || m_Texture.GetWidth() == 0)
		{
			ASSERT(false, "Mesh cannot draw. It is empty");
		}
		shader->Bind();
		if (m_Texture.GetFilePath() != "") {
			m_Texture.Bind();
			shader->SetUniform1i("u_Texture", 0);
			shader->SetUniform1i("u_UseTexture", 1);
		} else {
			shader->SetUniform3f("u_Color", m_Color.x, m_Color.y, m_Color.z);
			shader->SetUniform1i("u_UseTexture", 0);
		}
		shader->SetUniformMat4f("u_Model", model);
		
		Renderer renderer;
		renderer.Draw(m_VertexArray, m_IndexBuffer, shader);
	}
	void Mesh::SetData(const void * vertexData, unsigned int vertexBufferSize, const unsigned int * indexData, unsigned int indexSize, const Texture& texture, Vector3 color)
	{
		m_VertexBuffer.SetBufferData(vertexData, vertexBufferSize);
		m_IndexBuffer.SetBufferData(indexData, indexSize);
		if (texture.GetFilePath() != "")
			m_Texture.LoadTexture(texture.GetFilePath());
		m_Color = color;
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

