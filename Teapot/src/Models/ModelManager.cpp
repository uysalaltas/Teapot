#include "ModelManager.h"

namespace Teapot
{
	size_t ModelManager::GetModelVectorSize()
	{
		size_t size = 0;
		for (const auto& modelVec : s_AllModels)
		{
			size += modelVec->size();
		}
		return size;
	}

	std::vector<ModelInterfacePtr> Teapot::ModelManager::GetModels()
	{
		std::vector<ModelInterfacePtr> result;
		size_t total = 0;
		for (const auto& vec : s_AllModels)
			if (vec) total += vec->size();
		result.reserve(total);
		for (const auto& vec : s_AllModels)
			if (vec) result.insert(result.end(), vec->begin(), vec->end());
		return result;
	}

	void ModelManager::SetSelectedModel(ModelInterfacePtr modelInterfacePtr)
	{
		s_SelectedModel = modelInterfacePtr;
	}

	std::shared_ptr<Teapot::ModelInterface> ModelManager::GetSelectedModel()
	{
		if (s_SelectedModel.expired())
		{
			for (size_t i = 0; i < s_AllModels.size(); i++)
			{
				for (size_t j = 0; j < s_AllModels[i]->size(); j++)
				{
					SetSelectedModel((*s_AllModels[i])[j]);
					return s_SelectedModel.lock();
				}
			}
			return std::shared_ptr<Teapot::ModelInterface>(nullptr);
		}

		return s_SelectedModel.lock();
	}

	void ModelManager::PushModel(const ModelVectorPtr& modelVec)
	{
		s_AllModels.push_back(modelVec);
	}

	void ModelManager::RemoveSelectedModel()
	{
		for (const auto& modelVec : s_AllModels)
		{
			unsigned int modelVecIdx{};
			for (const auto& model : *modelVec)
			{
				if (model == GetSelectedModel()) 
				{ 
					modelVec->erase(modelVec->begin() + modelVecIdx);
					return;
				}
				modelVecIdx++;
			}
		}
	}
}
