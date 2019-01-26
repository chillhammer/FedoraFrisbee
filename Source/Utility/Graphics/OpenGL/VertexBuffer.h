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
		const void* m_Data;
		unsigned int m_Size;
	public:
		VertexBuffer();
		VertexBuffer(const void* data, unsigned int size);
		VertexBuffer(const VertexBuffer& other);
		~VertexBuffer();

		void SetBufferData(const void* data, unsigned int size);
		bool IsEmpty() const;
		void Bind() const;
		void Unbind() const;
	};
}