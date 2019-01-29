#pragma once
#include "Mesh.h"
namespace Fed {
	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoords;
	};
	/**
		Specific mesh designed for import.
	**/
	class ImportedMesh :
		public Mesh
	{
	public:
		ImportedMesh(std::vector<Vertex> vertexBuffer, std::vector<unsigned int> indexBuffer,
			Texture texture);
		ImportedMesh(std::vector<Vertex> vertexBuffer, std::vector<unsigned int> indexBuffer,
			Vector3 color);
		ImportedMesh(const ImportedMesh& other);
		virtual ~ImportedMesh();
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indicies;

		virtual VertexBufferLayout GetBufferLayout() override;
	};
}