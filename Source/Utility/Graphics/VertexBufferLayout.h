#pragma once
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Fed
{
	// Element to describe attribute within VertexBufferLayout
	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type)
		{
			switch (type)
			{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
			}
			// Error
			return 0;
		}
	};

	// Describes the information a vertex contains in a Vertex Buffer
	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Elements;
		unsigned int m_Stride;
	public:
		VertexBufferLayout() : m_Stride(0) {};

		// Fail on undeclared type
		template<typename T>
		void Push(unsigned int count)
		{
			ASSERT(false);
		}
		
		// Specified types
		template<>
		void Push<float>(unsigned int count)
		{
			m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
			m_Stride += sizeof(GLfloat) * count;
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
			m_Stride += sizeof(GLuint) * count;
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
			m_Stride += sizeof(GLbyte) * count;
		}

		inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
		inline unsigned int GetStride() const { return m_Stride; }
	};
}