#pragma once
#include <memory>
#include <functional>

#include "ModelManager.h"

namespace Teapot
{
	inline const std::string SHADERPATH = "Teapot/shaders/";

	class ModelHandlerInterface
	{
	public:
		ModelHandlerInterface()
		{
			s_Models = std::make_shared<ModelVector>();
			Teapot::ModelManager::PushModel(s_Models);
			m_runAwakeFuncVec.emplace_back([this]() {this->RunAwake(); });
			m_drawModelsFuncVec.emplace_back([this]() {this->DrawModels(); });
		}

		virtual ~ModelHandlerInterface() = default;

		virtual std::shared_ptr<Teapot::ModelInterface> CreateModel(Shapes::Shape& shapes, const std::string& nameObject) = 0;
		virtual std::shared_ptr<Teapot::ModelInterface> CreateModel(const std::string& pathObject, const std::string& nameObject) = 0;
		virtual void RunAwake() = 0;
		virtual void DrawModels() = 0;

		std::string modelHandlerName{};

		ModelVectorPtr s_Models;

		inline static void RunAwakeModels()
		{
			for (const auto& runAwake : m_runAwakeFuncVec)
			{
				runAwake();
			}
		}
		
		inline static void RunDrawModels() 
		{
			for (const auto& drawModels : m_drawModelsFuncVec)
			{
				drawModels();
			}
		}

	private:
		inline static std::vector<std::function<void()>> m_runAwakeFuncVec;
		inline static std::vector<std::function<void()>> m_drawModelsFuncVec;
	};
}