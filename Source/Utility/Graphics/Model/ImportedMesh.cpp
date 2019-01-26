#include <FedPCH.h>
#include "ImportedMesh.h"

namespace Fed
{
	ImportedMesh::ImportedMesh(std::vector<Vertex> vertexBuffer, std::vector<unsigned int> indexBuffer, Texture texture)
		:	m_Vertices(vertexBuffer),
			m_Indicies(indexBuffer),
			Mesh()
	{
		SetBufferLayout(GetBufferLayout());
		SetData(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex), &m_Indicies[0], m_Indicies.size(), texture);
	}

	ImportedMesh::ImportedMesh(const ImportedMesh & other)
		:	m_Vertices(other.m_Vertices),
			m_Indicies(other.m_Indicies),
			Mesh()
	{
		SetBufferLayout(GetBufferLayout());
		SetData((void*)&m_Vertices[0], m_Vertices.size() * sizeof(Vertex), &m_Indicies[0], m_Indicies.size(), other.m_Texture);
	}
	ImportedMesh::~ImportedMesh()
	{
	}
	VertexBufferLayout ImportedMesh::GetBufferLayout()
	{
		VertexBufferLayout vertexBufferLayout;
		vertexBufferLayout.Push<float>(3);	// Position
		vertexBufferLayout.Push<float>(3);	// Normals
		vertexBufferLayout.Push<float>(2);	// Texture Coords
		return vertexBufferLayout;
	}
}