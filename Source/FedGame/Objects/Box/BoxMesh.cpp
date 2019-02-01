#include <FedPCH.h>
#include "BoxMesh.h"

namespace Fed
{
	//	Position(3) - Normal(3) - Texture Coord(2)
	static float positions[] = {
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,	//Bottom Left
		 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,	//Bottom Right
		 0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,	//Top Right
		-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,	//Top Left

		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //Side Bottom Left
		 0.5f, -0.5f,-1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //Side Bottom Right
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //Side Top Left
		 0.5f,  0.5f,-1.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //Side Top Right

		-0.5f, -0.5f, 0.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 0.0f, //Side Bottom Right		8
		-0.5f, -0.5f,-1.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 0.0f, //Side Bottom Left
		-0.5f,  0.5f, 0.0f,  -1.0f, 0.0f, 0.0f,  0.0f, 1.0f, //Side Top Right
		-0.5f,  0.5f,-1.0f,  -1.0f, 0.0f, 0.0f,  1.0f, 1.0f, //Side Top Left

		-0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f, //Bottom Bottom Left	12
		 0.5f, -0.5f, 0.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f, //Bottom Bottom Right
		-0.5f, -0.5f,-1.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, //Bottom Top Left
		 0.5f, -0.5f,-1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f, //Bottom Top Right

		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, //Top Bottom Left		16
		 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, //Top Bottom Right
		-0.5f,  0.5f,-1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, //Top Top Left
		 0.5f,  0.5f,-1.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, //Top Top Right

		-0.5f, -0.5f,-1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,	 //Back Bottom Left		20
		 0.5f, -0.5f,-1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 0.0f,	 //Back Bottom Right
		 0.5f,  0.5f,-1.0f,  0.0f, 0.0f, -1.0f,  1.0f, 1.0f,	 //Back Top Right
		-0.5f,  0.5f,-1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 1.0f,	 //Back Top Left
	};
	static unsigned int indicies[] = {
		0, 1, 2,	//Front
		2, 3, 0,

		4, 5, 6,	//Right Side
		7, 6, 5,

		10, 9, 8,	//Left Side
		10, 11, 9,

		12, 14, 13,	//Bottom
		14, 15, 13,

		16, 17, 18,	//Top
		17, 19, 18,

		20, 23, 21,	//Back
		21, 23, 22
	};
	static int sides = 6;

	BoxMesh::BoxMesh(Texture& texture) 
		: Mesh(positions, sizeof(float) * 8 * (4 * sides), indicies, (6 * sides), texture)
	{
	}

	 BoxMesh::~BoxMesh()
	 {
	 }
}