#pragma once
#include <FedPCH.h>
#include <Graphics/OpenGL/Shader.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ImportedMesh.h"
namespace Fed
{
	/**
		3D model class that encompasses multiple meshes.
	**/
	class Model
	{
	public:
		/*  Functions   */
		Model(char *path)
		{
			LoadModel(path);
		}
		void Draw(Shader& shader);
	private:
		/*  Model Data  */
		std::vector<ImportedMesh> m_Meshes;
		std::string m_Directory;
		/*  Functions   */
		void LoadModel(std::string path);
		void ProcessNode(aiNode *node, const aiScene *scene);
		ImportedMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
		Texture LoadMaterialTexture(aiMaterial *mat, aiTextureType type);
	};
}