#pragma once
namespace Fed
{
	/**
		OpenGL wrapper to contain information about vertices.
		Information could be position, normals, texture coords, etc.
	**/
	class VertexBuffer
	{
	private:
		unsigned int m_RendererID;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
	};
}