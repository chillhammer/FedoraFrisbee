#include <FedPCH.h>
#include "ResourceManager.h"
#include <Graphics/Model/ImportedModel.h>
#include <Objects/Box/BoxMesh.h>
#include <Objects/Box/WoodenBoxModel.h>

namespace Fed
{
	ResourceManager & Fed::ResourceManager::Instance()
	{
		static ResourceManager resources;
		return resources;
	}
	void ResourceManager::Init()
	{
		LoadTextures();
		LoadMeshes();
		LoadModels();
		LoadShaders();
		LOG("Initialized Resource Manager");
	}
	TexturePtr ResourceManager::GetTexture(std::string name)
	{
		ASSERT(m_TextureTable[name], "Cannot load texture: " + name);
		return m_TextureTable[name];
	}
	MeshPtr ResourceManager::GetMesh(std::string name)
	{
		ASSERT(m_MeshTable[name], "Cannot load mesh: " + name);
		return m_MeshTable[name];
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
	void ResourceManager::LoadTextures()
	{
		TexturePtr wood(new Texture("../Assets/Textures/wood.png"));
		wood->LoadTexture();
		m_TextureTable.emplace("Wood", wood);
	}
	void ResourceManager::LoadMeshes()
	{
		m_MeshTable.emplace("WoodenBox", new BoxMesh(*GetTexture("Wood")));
	}
	void ResourceManager::LoadModels()
	{
		m_ModelTable.emplace("Court",	new ImportedModel("../Assets/Models/Court/court.obj"));
		m_ModelTable.emplace("SuitBlue",	new ImportedModel("../Assets/Models/Suit/suit-blue.obj"));
		m_ModelTable.emplace("SuitRed",	new ImportedModel("../Assets/Models/Suit/suit-red.obj"));
		m_ModelTable.emplace("FedoraAgent",	new ImportedModel("../Assets/Models/FedoraAgent/fedora-agent.obj"));
		m_ModelTable.emplace("Fedora",	new ImportedModel("../Assets/Models/Fedora/fedora.obj"));
		m_ModelTable.emplace("WoodenBox",	new WoodenBoxModel());
	}
	void ResourceManager::LoadShaders()
	{
		m_ShaderTable.emplace("Basic", new Shader("Shaders/Basic.shader"));
		m_ShaderTable.emplace("Model", new Shader("Shaders/Model.shader"));
		m_ShaderTable.emplace("Debug", new Shader("Shaders/Debug.shader"));
		m_ShaderTable.emplace("UI", new Shader("Shaders/UI.shader"));
	}
}