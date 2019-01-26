#pragma once
#include <Graphics/Model/Mesh.h>
#include <Graphics/OpenGL/Texture.h>
namespace Fed
{
	/**
		Simple implementation of Mesh. A Box.
	**/
	class BoxMesh : public Mesh
	{
	public:
		BoxMesh(Texture& texture);
		~BoxMesh();

	private:

	};
}