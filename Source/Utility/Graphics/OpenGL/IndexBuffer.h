#pragma once
namespace Fed
{
	/**
		OpenGL wrapper containing indicies to create a shape.
	**/
	class IndexBuffer
	{
	private:
		unsigned int m_RendererID;
		const unsigned int* m_Data;
		unsigned int m_Count;
	public:
		IndexBuffer();
		IndexBuffer(const unsigned int* data, unsigned int count);
		IndexBuffer(const IndexBuffer& other);
		~IndexBuffer();

		void SetBufferData(const unsigned int* data, unsigned int count);
		bool IsEmpty();
		void Bind() const;
		void Unbind() const;

		inline unsigned int GetCount() const { return m_Count; }
	};
}