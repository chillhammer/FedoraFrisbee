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
		GLCall(glClearColor(0.55f, 0.78f, 0.95f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	// Draws onto screen
	void Renderer::Draw(const VertexArray & va, const IndexBuffer & ib, const ShaderPtr & shader) const
	{
		shader->Bind();
		va.Bind();
		ib.Bind();
		
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}
}
