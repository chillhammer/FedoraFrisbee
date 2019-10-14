#include "FedPCH.h"
#include <stb_image/stb_image.h>
#include "Cubemap.h"

namespace Fed {
	Cubemap::Cubemap(const std::string& path) : Texture(path)
	{
		// Texture is loaded in ResourcesManager
	}
	void Cubemap::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID));
	}

	void Cubemap::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	}
	// Expected to be concatenated with '-1.png', '-2.png'
	void Cubemap::LoadTexture(const std::string& path)
	{
		m_FilePath = path;
		stbi_set_flip_vertically_on_load(0);
		Bind();
		for (int i = 0; i < 6; ++i) {
			std::string facePath = path + "-" + std::to_string(i) + ".png";
			m_LocalBuffer = stbi_load(facePath.c_str(), &m_Width, &m_Height, &m_BPP, 0);
			ASSERT(m_LocalBuffer, "Failed to load texture");
			LOG("Sucessfully loaded cubemap texture: {0}", facePath);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_LocalBuffer);
			stbi_image_free(m_LocalBuffer);
		}


		// Setting OpenGL Parameters
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

		// Unbind
		Unbind();
		
	}
}

