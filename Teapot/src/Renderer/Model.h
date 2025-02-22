#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer/Renderer.h"
#include "ShapeGenerator/Shapes.h"

namespace Teapot
{
	enum class ShapeObjects
	{
		Custom   = 0,
		PathObj  = 1,
		Cube     = 2,
		Cylinder = 3,
		Plane    = 4,
		Sphere   = 5
	};

	class Model
	{
	public:
		Model(const std::string& pathObject, const std::string& nameObject);
		Model(const Shapes::Shape& shapes, const std::string& nameObject, const ShapeObjects type);

		void Draw() const;
		void DrawShadow() const;
		void Translate(const glm::vec3& translation);
		void Rotate(const glm::vec3& rotation);
		void Scale(const glm::vec3& scale);
		void Manipulate();
		void LoadTextureToModel(const std::string& textureType, const std::string& texturePath, int unit);

		static std::shared_ptr<Model> CreateModel(const Shapes::Shape& shapes, const std::string& nameObject, const ShapeObjects type = ShapeObjects::Custom);
		static std::shared_ptr<Model> CreateModel(const std::string& pathObject, const std::string& nameObject);
		static void RemoveModel();

	public:
		std::string path;
		std::string name;
		glm::vec3 modelColor = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::mat4 objModel = glm::mat4(1.0f);
		glm::vec3 objRotation = glm::vec3(0.0f);
		glm::vec3 objScale = glm::vec3(1.0f);
		glm::vec3 objTranslation = glm::vec3(0.0f);
		std::vector<std::unique_ptr<Renderer>> meshes;
		ShapeObjects shapeType;

	private:
		void LoadModel(const std::string& modelPath);
		void ProcessNode(const aiNode* node, const aiScene* scene);
		std::unique_ptr<Renderer> ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName);

	private:
		std::string m_directory;
		std::vector<Texture> m_texturesLoaded;
		bool m_HasTexture{false};
	};

	class ModelManager
	{
	public:
		ModelManager() = default;

		inline static unsigned int GetModelVectorSize() { return s_Models.size(); }
		inline static std::shared_ptr<Model> GetSelectedModel() { return s_Models[s_SelectedModel]; }
		inline static std::shared_ptr<Model> GetModel(int idx) {return s_Models[idx];}
		inline static int GetSelectedModelIndex() { return s_SelectedModel; }
		inline static void SetSelectedModelIndex(int idx) { s_SelectedModel = idx; }
		inline static std::vector<std::shared_ptr<Model>> GetModelVector() { return s_Models; }

		inline static void DrawModelShadows()
		{
			for (const auto& model : s_Models)
			{
				model->DrawShadow();
			}
		};

		inline static void DrawModels()
		{
			for (const auto& model : s_Models)
			{
				model->Draw();
			}
		};

		friend class Model;

	private:
		inline static std::vector<std::shared_ptr<Model>> s_Models;
		inline static int s_SelectedModel{ 0 };
	};
}