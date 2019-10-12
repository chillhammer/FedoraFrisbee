#include <FedPch.h>
#include "Sprite.h"

namespace Fed {

	static float vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,		// Top Right
		1.0f, 0.0f, 1.0f, 0.0f,		// Bottom Left
		0.0f, 0.0f, 0.0f, 0.0f,		// Top Left
		1.0f, 1.0f, 1.0f, 1.0f		// Bottom Right
	};

	static unsigned int indices[] = {
		0, 1, 2,
		0, 3, 1
	};


	Sprite::Sprite( TexturePtr texture)
		: m_VertexBuffer(vertices, sizeof(vertices)),
		m_IndexBuffer(indices, sizeof(indices)),
		m_Texture(*texture)
	{
		m_BufferLayout.Push<float>(2); // Position
		m_BufferLayout.Push<float>(2); // Texture Coords
		m_VertexArray.AddBuffer(m_VertexBuffer, m_BufferLayout);
	}



	Sprite::Sprite(const Sprite& other)
		: m_VertexBuffer(other.m_VertexBuffer),
		m_IndexBuffer(other.m_IndexBuffer),
		m_Texture(other.m_Texture),
		m_Color(other.m_Color)
	{
		ASSERT(false, "Cannot copy over Base Sprite Object");
	}

	Sprite::~Sprite()
	{
	}
	void Sprite::Draw(const ShaderPtr& shader, const Matrix4x4& model)
	{
		if (m_VertexBuffer.IsEmpty() || m_IndexBuffer.IsEmpty())
		{
			ASSERT(false, "Mesh cannot draw. It is empty");
		}
		shader->Bind();
		if (m_Texture.GetFilePath() != "") {
			m_Texture.Bind();
			shader->SetUniform1i("u_Texture", 0);
			shader->SetUniform1i("u_UseTexture", 1);
		}
		else {
			shader->SetUniform3f("u_Color", m_Color.x, m_Color.y, m_Color.z);
			shader->SetUniform1i("u_UseTexture", 0);
		}
		shader->SetUniformMat4f("u_Model", model);

		Renderer renderer;
		renderer.Draw(m_VertexArray, m_IndexBuffer, shader);
	}
}