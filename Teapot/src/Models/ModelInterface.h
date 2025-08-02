#pragma once

#include "ShapeGenerator/Shapes.h"
#include "Renderer/Renderer.h"

namespace Teapot
{
	enum class ModelType
	{
		model = 0,
		debugModel = 1
	};

	class ModelInterface
	{
	public:
		virtual ~ModelInterface() = default;
		virtual void Draw(Teapot::Shader& shader) = 0;

		inline virtual void Translate(const glm::vec3& translation) { objTranslation = translation; Manipulate(); }
		inline virtual void Rotate(const glm::vec3& rotation) { objRotation = rotation; Manipulate(); }
		inline virtual void Scale(const glm::vec3& scale) { objScale = scale; Manipulate(); }
		inline virtual void Manipulate()
		{
			objModel = glm::translate(glm::mat4(1.0f), objTranslation);
			objModel = glm::rotate(objModel, glm::radians(objRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			objModel = glm::rotate(objModel, glm::radians(objRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			objModel = glm::rotate(objModel, glm::radians(objRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			objModel = glm::scale(objModel, objScale);
		}

		std::string name{};
		ModelType modelType{};
		std::string path{};
		bool hasTexture{};

		glm::mat4 objModel{ glm::mat4(1.0f) };
		glm::vec3 modelColor{ glm::vec3(1.0f) };
		glm::vec3 objRotation{ glm::vec3(0.0f) };
		glm::vec3 objScale{ glm::vec3(1.0f) };
		glm::vec3 objTranslation{ glm::vec3(0.0f) };

		std::vector<std::unique_ptr<Teapot::Renderer>> meshes;
	};
}