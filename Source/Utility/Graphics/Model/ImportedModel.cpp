#include <FedPCH.h>
#include "ImportedModel.h"
namespace Fed
{
	// Imports model through given file path
	ImportedModel::ImportedModel(char* path)
	{
		LoadModel(path);
		LOG("Imported Model: {0}", path);
	}

	// Loops Through All Meshes, Then Draws
	void ImportedModel::Draw(const ShaderPtr& shader, const Matrix4x4& model)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Draw(shader, model);
	}

	// Base function to load in model
	void ImportedModel::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			LOG_ERROR("ASSIMP::{0}", importer.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	// Finds ands processes all meshes
	void ImportedModel::ProcessNode(aiNode * node, const aiScene * scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
			m_Meshes.push_back(ProcessMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	ImportedMesh ImportedModel::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		// Parameters for ImportedMesh
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		// Get Vertex Data
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			vertex.Position =
			{
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};
			vertex.Normal =
			{
					mesh->mNormals[i].x,
					mesh->mNormals[i].y,
					mesh->mNormals[i].z
			};
			// Get TextureCoordinates if Exists
			if (mesh->mTextureCoords[0])
			{
				vertex.TexCoords =
				{
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				};
			}
			else
				vertex.TexCoords = { 0.f, 0.f };
			vertices.push_back(vertex);
		}

		// Get Index Data
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Get Texture or Color based on Material
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		Texture diffuseTexture = LoadMaterialTexture(material, aiTextureType_DIFFUSE);
		if (diffuseTexture.GetFilePath() == "")
		{
			aiColor3D color;
			ASSERT(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color), "Failed to find color or texture");
			aiString name;
			ASSERT(AI_SUCCESS == material->Get(AI_MATKEY_NAME, name), "Failed to get color name");
			LOG("Sucessfully loaded color material: {0}", name.C_Str());
			return ImportedMesh(vertices, indices, Vector3(color.r, color.g, color.b));
		}
		return ImportedMesh(vertices, indices, diffuseTexture);
	}

	// Creates texture based on file path
	Texture ImportedModel::LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
	{
		aiString str;
		if (AI_SUCCESS == mat->GetTexture(type, 0, &str))
		{
			LOG("Sucessfully loaded texture material: {0}", m_Directory + '/' + str.C_Str());
			return Texture(m_Directory + '/' + str.C_Str());
		}
		return Texture();
	}
}