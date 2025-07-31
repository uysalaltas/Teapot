#pragma once
#include <memory>
#include <ranges>

#include "ModelInterface.h"

namespace Teapot
{
	using ModelInterfacePtr = std::shared_ptr<Teapot::ModelInterface>;
	using ModelVector = std::vector<ModelInterfacePtr>;
	using ModelVectorPtr = std::shared_ptr<ModelVector>;

	class ModelManager
	{
	public:
		ModelManager() = default;

		static size_t GetModelVectorSize();
		static std::vector<ModelInterfacePtr> GetModels();
		static void SetSelectedModel(ModelInterfacePtr modelInterfacePtr);
		static std::shared_ptr<Teapot::ModelInterface> GetSelectedModel();
		static void PushModel(const ModelVectorPtr& modelVec);
		static void RemoveSelectedModel();

	private:

		inline static std::vector<ModelVectorPtr> s_AllModels;
		inline static std::weak_ptr<Teapot::ModelInterface> s_SelectedModel;
	};
}