#include "ModelManager.h"

namespace Teapot
{
	size_t ModelManager::GetModelVectorSize()
	{
		size_t size = 0;
		for (const auto& modelVec : sv_allModels)
		{
			size += modelVec->size();
		}
		return size;
	}

	std::vector<ModelInterfacePtr> Teapot::ModelManager::GetModels()
	{
		std::vector<ModelInterfacePtr> result;
		size_t total = 0;
		for (const auto& vec : sv_allModels)
		{
			if (vec) total += vec->size();
		}
		result.reserve(total);
		for (const auto& vec : sv_allModels)
		{
			if (vec) result.insert(result.end(), vec->begin(), vec->end());
		}
		return result;
	}

	void ModelManager::SetSelectedModel(ModelInterfacePtr modelInterfacePtr)
	{
		sv_selectedModel = modelInterfacePtr;
	}

	std::shared_ptr<Teapot::ModelInterface> ModelManager::GetSelectedModel()
	{
		if (sv_selectedModel.expired())
		{
			for (size_t i = 0; i < sv_allModels.size(); i++)
			{
				for (size_t j = 0; j < sv_allModels[i]->size(); j++)
				{
					SetSelectedModel((*sv_allModels[i])[j]);
					return sv_selectedModel.lock();
				}
			}
			return std::shared_ptr<Teapot::ModelInterface>(nullptr);
		}

		return sv_selectedModel.lock();
	}

	void ModelManager::PushModel(const ModelVectorPtr& modelVec)
	{
		sv_allModels.push_back(modelVec);
	}

	void ModelManager::RemoveSelectedModel()
	{
		for (const auto& modelVec : sv_allModels)
		{
			size_t modelVecIdx{};
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
