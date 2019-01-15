#pragma once
#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
namespace Fed
{
	/**
		Combines all other OpenGL wrappers to draw onto screen.
	**/
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Clear() const;
		void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	};
}