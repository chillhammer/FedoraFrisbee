#include <FedPCH.h>
#include "ResourceManager.h"
#include <Graphics/Model/ImportedModel.h>

namespace Fed
{
	ResourceManager & Fed::ResourceManager::Instance()
	{
		static ResourceManager resources;
		return resources;
	}
	void ResourceManager::Init()
	{
		LoadModels();
		LOG("Initialized Resource Manager");
	}
	ModelPtr ResourceManager::GetModel(std::string name)
	{
		return m_ModelTable[name];
	}
	void ResourceManager::LoadModels()
	{
		m_ModelTable.emplace("RoboPadron", new ImportedModel("../Assets/Models/robopadron.obj"));
	}
}