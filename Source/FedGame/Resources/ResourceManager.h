#pragma once
#include <unordered_map>
#include <Graphics/Model/Model.h>
#include <Graphics/Model/ImportedModel.h>

#define Resources ResourceManager::Instance()
typedef std::shared_ptr<Fed::Model> ModelPtr;

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
		ModelPtr GetModel(std::string name);
	private:
		ResourceManager() {};
		void LoadModels();

		std::unordered_map<std::string, ModelPtr> m_ModelTable;
	};
}