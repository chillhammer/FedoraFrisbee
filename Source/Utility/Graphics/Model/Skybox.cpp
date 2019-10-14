#include "FedPCH.h"
#include <Resources/ResourceManager.h>
#include "Skybox.h"

namespace Fed
{
	//float skyboxVertices[] = {
	//	// positions          
	//	-1.0f,  1.0f, -1.0f,	// Back Top Left
	//	-1.0f, -1.0f, -1.0f,	// Back Bottom Left
	//	 1.0f, -1.0f, -1.0f,	// Back Bottom Right
	//	 1.0f, -1.0f, -1.0f,	
	//	 1.0f,  1.0f, -1.0f,	// Back Top Right
	//	-1.0f,  1.0f, -1.0f,

	//	-1.0f, -1.0f,  1.0f,	// Front Bottom Left
	//	-1.0f, -1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,
	//	-1.0f,  1.0f, -1.0f,	// 0
	//	-1.0f,  1.0f,  1.0f,	// Front Top Left
	//	-1.0f, -1.0f,  1.0f,	

	//	 1.0f, -1.0f, -1.0f,	// 2
	//	 1.0f, -1.0f,  1.0f,	// Front Bottom Right
	//	 1.0f,  1.0f,  1.0f,	// Front Top Right
	//	 1.0f,  1.0f,  1.0f,	// 7
	//	 1.0f,  1.0f, -1.0f,	// 3
	//	 1.0f, -1.0f, -1.0f,	// 2

	//	-1.0f, -1.0f,  1.0f,	// 4
	//	-1.0f,  1.0f,  1.0f,	// 5
	//	 1.0f,  1.0f,  1.0f,	// 7
	//	 1.0f,  1.0f,  1.0f,	// 7
	//	 1.0f, -1.0f,  1.0f,	// 6
	//	-1.0f, -1.0f,  1.0f,	// 4

	//	-1.0f,  1.0f, -1.0f,	// 0
	//	 1.0f,  1.0f, -1.0f,	// 3
	//	 1.0f,  1.0f,  1.0f,	// 7
	//	 1.0f,  1.0f,  1.0f,	// 7
	//	-1.0f,  1.0f,  1.0f,	// 4
	//	-1.0f,  1.0f, -1.0f,	// 0

	//	-1.0f, -1.0f, -1.0f,	// 1
	//	-1.0f, -1.0f,  1.0f,	// 4
	//	 1.0f, -1.0f, -1.0f,	// 2
	//	 1.0f, -1.0f, -1.0f,	// 2
	//	-1.0f, -1.0f,  1.0f,	// 4
	//	 1.0f, -1.0f,  1.0f		// 6
	//};

	static float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,	// Back Top Left
		-1.0f, -1.0f, -1.0f,	// Back Bottom Left
		 1.0f, -1.0f, -1.0f,	// Back Bottom Right
		 1.0f,  1.0f, -1.0f,	// Back Top Right
		-1.0f, -1.0f,  1.0f,	// Front Bottom Left
		-1.0f,  1.0f,  1.0f,	// Front Top Left
		 1.0f, -1.0f,  1.0f,	// Front Bottom Right
		 1.0f,  1.0f,  1.0f		// Front Top Right
	};

	static unsigned int skyboxIndices[] = {
		0, 1, 2,	// Back		// Original: 0, 1, 2
		2, 3, 0,

		4, 1, 0,	// Side left
		0, 5, 4,

		2, 6, 7,	// Side right
		7, 3, 2,

		4, 5, 7,	// Front
		7, 6, 4,

		0, 3, 7,	// Top
		7, 4, 0,

		1, 4, 2,	// Bottom
		2, 4, 6 
	};

	// Backwards
	//unsigned int skyboxIndices[] = {
	//	2, 1, 0,	// Back		// Original: 0, 1, 2
	//	0, 3, 2,

	//	0, 1, 4,	// Side left
	//	4, 5, 0,

	//	7, 6, 2,	// Side right
	//	2, 3, 7,

	//	7, 5, 4,	// Front
	//	4, 6, 7,

	//	7, 3, 0,	// Top
	//	0, 4, 7,

	//	2, 4, 1,	// Bottom
	//	6, 4, 2
	//};

	Skybox::Skybox() : m_VertexData(skyboxVertices), m_Cubemap(Resources.GetTexture("Skybox")), m_VertexArray(), 
		m_IndexBuffer(skyboxIndices, sizeof(skyboxIndices) / sizeof(unsigned int)), 
		m_Buffer(m_VertexData, 24 * sizeof(float)),
		m_VertexBufferLayout()
	{
		//VertexBufferLayout m_VertexBufferLayout;
		m_VertexBufferLayout.Push<float>(3);

		m_VertexArray.AddBuffer(m_Buffer, m_VertexBufferLayout);
	}
	void Skybox::Draw()
	{
		ShaderPtr skyboxShader = Resources.GetShader("Skybox");

		skyboxShader->Bind();
		m_Cubemap->Bind();

		//Resources.GetTexture("Wood")->Bind();

		//Matrix4x4 model(1.0f);

		skyboxShader->SetUniform1i("u_Skybox", 0);
		//skyboxShader->SetUniformMat4f("u_Model", model);

		glDepthFunc(GL_LEQUAL);

		Renderer renderer;
		renderer.Draw(m_VertexArray, m_IndexBuffer, skyboxShader);

		glDepthFunc(GL_LESS);
	}
}
