#pragma once
#include <unordered_map>
#include <Graphics/Model/Mesh.h>
#include <Graphics/Model/Model.h>
#include <Graphics/OpenGL/Texture.h>
#include <Graphics/Text/Font.h>

#define Resources ResourceManager::Instance()

namespace Fed
{
	/**
		Loads in assets to be used in the game.
		A singleton that retrieves models, shaders, etc.
	**/
	class ResourceManager
	{
	public:
		static ResourceManager& Instance();
		void Init();
		TexturePtr	GetTexture(std::string name);
		MeshPtr		GetMesh(std::string name);
		ModelPtr	GetModel(std::string name);
		ShaderPtr	GetShader(std::string name);
		FontPtr		GetFont(std::string name);
	private:
		ResourceManager() {};
		void LoadTextures();
		void LoadMeshes();
		void LoadModels();
		void LoadShaders();
		void LoadFont();

		std::unordered_map<std::string, TexturePtr> m_TextureTable;
		std::unordered_map<std::string, MeshPtr> m_MeshTable;
		std::unordered_map<std::string, ModelPtr> m_ModelTable;
		std::unordered_map<std::string, ShaderPtr> m_ShaderTable;
		std::unordered_map<std::string, FontPtr> m_FontTable;
	};
}