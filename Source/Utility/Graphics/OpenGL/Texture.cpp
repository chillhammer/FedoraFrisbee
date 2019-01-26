#include <FedPCH.h>
#include "Texture.h"
#include <GL/glew.h>
#include <stb_image/stb_image.h>

namespace Fed
{
	Texture::Texture()
	{
		GLCall(glGenTextures(1, &m_RendererID));
	}
	Texture::Texture(const std::string & path) : m_FilePath(path), m_LocalBuffer(nullptr),
		m_Height(0), m_Width(0), m_BPP(0)
	{
		GLCall(glGenTextures(1, &m_RendererID));
		LoadTexture(m_FilePath);
	}

	// TODO: Optimize Textur copy constructor
	Texture::Texture(const Texture & other) : m_FilePath(other.m_FilePath), m_LocalBuffer(nullptr),
		m_Height(0), m_Width(0), m_BPP(0)
	{
		GLCall(glGenTextures(1, &m_RendererID));
		LoadTexture(other.m_FilePath);
	}

	Texture::~Texture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void Texture::LoadTexture(const std::string & path)
	{
		m_FilePath = path;
		stbi_set_flip_vertically_on_load(1);
		m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

		// Bind
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		// Setting OpenGL Parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height,
			0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

		// Unbind and free local buffer
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		if (m_LocalBuffer)
			stbi_image_free(m_LocalBuffer);
	}

	void Texture::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}
}