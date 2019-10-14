#pragma once
#include "Texture.h"

namespace Fed
{
	class Cubemap : public Texture
	{
	public:
		Cubemap(const std::string& path);
		virtual void Bind(unsigned int slot = 0) const override;
		virtual void Unbind() const override;

		virtual void LoadTexture(const std::string& path);
	};
}
