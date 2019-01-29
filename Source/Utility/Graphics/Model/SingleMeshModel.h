#pragma once
#include "Model.h"
#include "Mesh.h"

namespace Fed
{
	/**
		A Model that simply wraps one Mesh class.
		Allows to be referenced amongst other models.
		Usage: SingleMeshModel box(BoxMesh());
	**/
	class SingleMeshModel
		: public Model
	{
	public:
		SingleMeshModel(Mesh& mesh) : m_Mesh(mesh) {};
		void Draw(Shader& shader) { m_Mesh.Draw(shader); };
	private:
		Mesh& m_Mesh;
	};
}