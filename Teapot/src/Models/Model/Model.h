#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Models/ModelInterface.h"

namespace Teapot
{
	class Model : public ModelInterface
	{
	public:
		Model(const std::string& pathObject, const std::string& nameObject);
		Model(Shapes::Shape& shapes, const std::string& nameObject);

		void Draw(Teapot::Shader& shader) override;
		void LoadTextureToModel(
			const std::string& textureType, 
			const std::string& texturePath, 
			int unit
		);

	private:
		void LoadModel(const std::string& modelPath);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		std::unique_ptr<Renderer> ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(
			std::vector<Texture>& textures, 
			const aiMaterial* mat, 
			const aiTextureType type, 
			const std::string& typeName
		);

	private:
		std::string m_directory;
	};
}