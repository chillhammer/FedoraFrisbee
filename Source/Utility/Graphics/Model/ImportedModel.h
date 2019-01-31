#pragma once
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ImportedMesh.h"

namespace Fed
{
	/**
		Imports 3D model based on .obj file.
		Parser and Importer relying on library Assimp.
		Currently will account for multiple meshes, but only 1 diffuse texture.
	**/
	class ImportedModel 
		: public Model
	{
	public:
		ImportedModel(char* path);
		void Draw(const ShaderPtr& shader, const Matrix4x4& model);
	private:
		std::vector<ImportedMesh> m_Meshes;

		#pragma region Loading Meshes Externally
		std::string m_Directory;
		void LoadModel(std::string path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		ImportedMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
		Texture LoadMaterialTexture(aiMaterial *mat, aiTextureType type);
		#pragma endregion
	};
}