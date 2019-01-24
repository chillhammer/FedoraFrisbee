#pragma once
#include <Graphics/OpenGL/Renderer.h>
#include <Graphics/OpenGL/Texture.h>
namespace Fed
{
	/**
		Simplest renderable entity
	**/
	class Mesh
	{
	public:
		Mesh(const void* vertexData, unsigned int vertexBufferSize,
			const unsigned int* indexData, unsigned int indexSize, Texture& texture);
		~Mesh();
		const VertexArray& GetVertexArray() const;
		const IndexBuffer& GetIndexBuffer() const;
		const Texture& GetTexture() const;
	protected:
		// Template pattern functions
		virtual VertexBufferLayout GetBufferLayout();
		Texture& m_Texture;
	private:
		void SetBufferLayout(VertexBufferLayout layout);
		const void* m_VertexData;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		VertexBufferLayout m_BufferLayout;
		VertexArray m_VertexArray;
	};

	namespace Msh
	{
		void DrawMesh(const Mesh& mesh, Shader& shader);
	}
}