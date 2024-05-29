#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Renderer.h"

namespace Teapot
{
	class Model
	{
	public:
		Model(std::string& pathObject, std::string& nameObject);
		Model(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& colors, std::vector<glm::vec3>& normals, std::vector<GLuint>& indices, std::string&& nameObject);

		//~Model();
		void Draw();
		void DrawShadow();
		void Translate(const glm::vec3& translation);
		void LoadTextureToModel(std::string& textureType, std::string& texturePath, int unit);

	public:
		std::string path;
		std::string name;
		glm::vec3 modelColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4 objModel = glm::mat4(1.0f);
		glm::vec3 objRotation = glm::vec3(0.0f);
		glm::vec3 objScale = glm::vec3(0.0f);
		glm::vec3 objTranslation = glm::vec3(0.0f);
		std::vector<std::unique_ptr<Renderer>> meshes;

		inline static std::vector<Model*> models;

	private:
		void LoadModel(std::string& path);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		Renderer ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, std::string& typeName);

	private:
		std::string directory;
		std::vector<Texture> texturesLoaded;
	};
}