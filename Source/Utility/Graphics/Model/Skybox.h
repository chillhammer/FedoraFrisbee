#pragma once
#include "Mesh.h"
#include "../OpenGL/Cubemap.h"

namespace Fed
{
	

	// Renders skybox as a custom box-mesh
	class Skybox
	{
	public:
		Skybox();
		void Draw();
	private:
		const void* m_VertexData;
		VertexBuffer m_Buffer;
		VertexBufferLayout m_VertexBufferLayout;
		IndexBuffer m_IndexBuffer;
		VertexArray m_VertexArray;
		TexturePtr m_Cubemap;
	};
}