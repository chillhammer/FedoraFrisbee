#pragma once
#include <unordered_map>
#include <Graphics/Model/Model.h>

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
		ModelPtr	GetModel(std::string name);
		ShaderPtr	GetShader(std::string name);
	private:
		ResourceManager() {};
		void LoadModels();
		void LoadShaders();

		std::unordered_map<std::string, ModelPtr> m_ModelTable;
		std::unordered_map<std::string, ShaderPtr> m_ShaderTable;
	};
}