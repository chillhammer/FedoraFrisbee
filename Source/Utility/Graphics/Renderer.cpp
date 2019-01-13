#include <FedPCH.h>
#include "Renderer.h"
#include "GL/glew.h"
namespace Fed
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::Clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// Draws onto screen
	void Renderer::Draw(VertexArray & va, const IndexBuffer & ib, const Shader & shader) const
	{
		shader.Bind();
		va.Bind();
		ib.Bind();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}