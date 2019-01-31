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
		Mesh();
		Mesh(const void* vertexData, unsigned int vertexBufferSize,
			const unsigned int* indexData, unsigned int indexSize, Texture texture);
		Mesh(const void* vertexData, unsigned int vertexBufferSize,
			const unsigned int* indexData, unsigned int indexSize, Vector3 color);
		Mesh(const Mesh& other);
		virtual ~Mesh();
		void Draw(const ShaderPtr& shader, const Matrix4x4& model);
	protected:
		void SetData(const void* vertexData, unsigned int vertexBufferSize,
			const unsigned int* indexData, unsigned int indexSize, const Texture& texture, Vector3 color);
		void SetBufferLayout(VertexBufferLayout layout);
		// Template pattern functions
		virtual VertexBufferLayout GetBufferLayout();

		Texture m_Texture;
		Vector3 m_Color;
	private:
		const void* m_VertexData;
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		VertexBufferLayout m_BufferLayout;
		VertexArray m_VertexArray;
	};
}