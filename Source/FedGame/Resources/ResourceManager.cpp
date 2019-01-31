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
		LoadShaders();
		LOG("Initialized Resource Manager");
	}
	ModelPtr ResourceManager::GetModel(std::string name)
	{
		ASSERT(m_ModelTable[name], "Cannot load model: " + name);
		return m_ModelTable[name];
	}
	ShaderPtr ResourceManager::GetShader(std::string name)
	{
		ASSERT(m_ShaderTable[name], "Cannot load shader: " + name);
		return m_ShaderTable[name];
	}
	void ResourceManager::LoadModels()
	{
		m_ModelTable.emplace("RoboPadron",	new ImportedModel("../Assets/Models/robopadron.obj"));
		//m_ModelTable.emplace("Suit",		new ImportedModel("../Assets/Models/Nanosuit/nanosuit.obj"));
	}
	void ResourceManager::LoadShaders()
	{
		m_ShaderTable.emplace("Basic", new Shader("Shaders/Basic.shader"));
		m_ShaderTable.emplace("Model", new Shader("Shaders/Model.shader"));
	}
}