#pragma once
#include <string>
namespace Fed
{
	/**
		OpenGL wrapper for texture which can be applied onto vertex buffer.
		Uses SOIL library to load in image.
	**/
	class Texture
	{
	protected:
		unsigned int m_RendererID;
		std::string m_FilePath;
		unsigned char* m_LocalBuffer;
		int m_Width, m_Height, m_BPP;
	public:
		Texture();
		Texture(const std::string& path);
		Texture(const Texture& other);
		~Texture();

		void LoadTexture();
		virtual void LoadTexture(const std::string& path);
		virtual void Bind(unsigned int slot = 0) const;
		virtual void Unbind() const;

		inline int GetWidth() const { return m_Width; }
		inline int GetHeight() const { return m_Height; }
		inline std::string GetFilePath() const { return m_FilePath; }
	};
	typedef std::shared_ptr<Texture> TexturePtr;
}