#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <future>

#include "Models/ModelInterface.h"

namespace Teapot
{
	class Model : public ModelInterface
	{
	public:
		Model(const std::string& pathObject, const std::string& nameObject, ModelType type);
		Model(Shapes::Shape& shapes, const std::string& nameObject, ModelType type);

		void Draw(Teapot::Shader& shader) override;
		void LoadTextureToModel(
			const std::string& textureType, 
			const std::string& texturePath, 
			int unit
		) const;

	private:
		void LoadModel(const std::string& modelPath);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		std::unique_ptr<Teapot::Renderer> ProcessMesh(const aiMesh* mesh, const aiScene* scene);
		void LoadMaterialTextures(
			std::vector<Teapot::Texture>& textures,
			const aiMaterial* mat, 
			const aiTextureType type, 
			const std::string& typeName
		);

	private:
		std::string m_directory;
	};
}