#pragma once
#include <Graphics/OpenGL/Renderer.h>
#include <Graphics/OpenGL/Texture.h>

namespace Fed 
{
	// Standard class for 2D images
	// Useful for UI
	class Sprite
	{
	public:
		
		Sprite(TexturePtr texture);
		Sprite(Vector3 color);
		Sprite(const Sprite& other);

		virtual ~Sprite();
		void Draw(const ShaderPtr& shader, const Matrix4x4& model);
		Texture m_Texture;
		Vector3 m_Color;
	private:
		Sprite() {};
		VertexBuffer m_VertexBuffer;
		IndexBuffer m_IndexBuffer;
		VertexBufferLayout m_BufferLayout;
		VertexArray m_VertexArray;
	};
	typedef std::shared_ptr<Sprite> SpritePtr;

}