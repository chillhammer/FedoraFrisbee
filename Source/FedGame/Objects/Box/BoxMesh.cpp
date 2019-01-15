#include <FedPCH.h>
#include "BoxMesh.h"

namespace Fed
{
	static float positions[] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	//Bottom Left
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	//Bottom Right
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,	//Top Right
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f	//Top Left
	};
	static unsigned int indicies[] = {
		0, 1, 2,
		2, 3, 0
	};

	BoxMesh::BoxMesh() 
		: Mesh(positions, sizeof(float) * 5 * 4, indicies, 6)
	{
	}

	 BoxMesh::~BoxMesh()
	 {
	 }
}