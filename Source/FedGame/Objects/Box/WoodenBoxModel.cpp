#include <FedPCH.h>
#include <Resources/ResourceManager.h>
#include "BoxMesh.h"
#include "WoodenBoxModel.h"

namespace Fed
{
	WoodenBoxModel::WoodenBoxModel() : SingleMeshModel(*Resources.GetMesh("WoodenBox"))
	{

	}
}