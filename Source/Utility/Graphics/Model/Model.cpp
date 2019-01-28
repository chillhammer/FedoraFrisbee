#include <FedPCH.h>
#include "Model.h"

namespace Fed
{
	void Model::Draw(Shader& shader)
	{
		for (unsigned int i = 0; i < m_Meshes.size(); i++)
			m_Meshes[i].Draw(shader);
	}
	void Model::LoadModel(std::string path)
	{
		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		{
			LOG_ERROR("ASSIMP::{0}", importer.GetErrorString());
			return;
		}
		m_Directory = path.substr(0, path.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}
	void Model::ProcessNode(aiNode * node, const aiScene * scene)
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
	ImportedMesh Model::ProcessMesh(aiMesh * mesh, const aiScene * scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		LOG("Process Mesh.");

		// Walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			Vector3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				Vector2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
			{
				LOG_WARN("Default tex coords");
				vertex.TexCoords = Vector2(0.0f, 0.0f);
			}
			//// tangent
			//vector.x = mesh->mTangents[i].x;
			//vector.y = mesh->mTangents[i].y;
			//vector.z = mesh->mTangents[i].z;
			//vertex.Tangent = vector;
			//// bitangent
			//vector.x = mesh->mBitangents[i].x;
			//vector.y = mesh->mBitangents[i].y;
			//vector.z = mesh->mBitangents[i].z;
			//vertex.Bitangent = vector;
			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// 1. diffuse maps
		Texture diffuseTexture = LoadMaterialTexture(material, aiTextureType_DIFFUSE);

		//Texture wood("../Assets/Textures/wood.png");
		return ImportedMesh(vertices, indices, diffuseTexture);
	}
	Texture Model::LoadMaterialTexture(aiMaterial* mat, aiTextureType type)
	{
		aiString str;
		mat->GetTexture(type, 0, &str);
		LOG("Trying to get texture: {0}", m_Directory + '/' + str.C_Str());
		return Texture(m_Directory + '/' + str.C_Str());
	}
}