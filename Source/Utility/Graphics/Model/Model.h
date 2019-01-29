#pragma once
#include <FedPCH.h>
#include <Graphics/OpenGL/Shader.h>
namespace Fed
{
	/**
		Interface to draw a 3D model.
		Model may encompass multiple meshes.
	**/
	class Model
	{
	public:
		virtual void Draw(Shader& shader) = 0;
	};
}