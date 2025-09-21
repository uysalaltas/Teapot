#pragma once
#include <memory>
#include <functional>
#include <future>

#include "ModelManager.h"
#include "Model/Model.h"

namespace Teapot
{
	inline const std::string SHADERPATH = "Teapot/shaders/";

	class ModelHandlerInterface
	{
	public:
		ModelHandlerInterface()
		{
			models = std::make_shared<ModelVector>();
			Teapot::ModelManager::PushModel(models);
			mv_runAwakeFuncVec.emplace_back([this]() {this->RunAwake(); });
			mv_drawModelsFuncVec.emplace_back([this]() {this->DrawModels(); });
		}

		virtual ~ModelHandlerInterface() = default;

		inline std::shared_ptr<Teapot::ModelInterface> CreateModel(Shapes::Shape& shapes, const std::string& nameObject)
		{
			auto model = std::make_shared<Teapot::Model>(shapes, nameObject, modelType);
			models->push_back(model);
			return model;
		}

		inline std::shared_ptr<Teapot::ModelInterface> CreateModel(const std::string& pathObject, const std::string& nameObject)
		{
			auto model = std::make_shared<Teapot::Model>(pathObject, nameObject, modelType);
			models->push_back(model);
			return model;
		}

		virtual void RunAwake() = 0;
		virtual void DrawModels() = 0;

		std::string modelHandlerName{};

		ModelVectorPtr models;
		ModelType modelType{};

		inline static void RunAwakeModels()
		{
			for (const auto& runAwake : mv_runAwakeFuncVec)
			{
				runAwake();
			}
		}
		
		inline static void RunDrawModels() 
		{
			for (const auto& drawModels : mv_drawModelsFuncVec)
			{
				drawModels();
			}
		}
	
	private:
		inline static std::vector<std::function<void()>> mv_runAwakeFuncVec;
		inline static std::vector<std::function<void()>> mv_drawModelsFuncVec;
	};
}