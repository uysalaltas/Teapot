#include "DebugModel.h"

namespace Teapot
{
	DebugModel::DebugModel(Shapes::Shape& shapes, const std::string& nameObject)
	{
		std::vector<Texture> textures;
		std::cout << nameObject << " Pos Size: " << shapes.positions.size() << std::endl;
		name = nameObject;
		modelType = ModelType::debugModel;
		meshes.push_back(std::make_unique<Renderer>(
			std::move(shapes.vertices), 
			std::move(shapes.indices), 
			std::move(textures))
		);
	}

	void DebugModel::Draw(Teapot::Shader& shader)
	{
		for (const auto& mesh : meshes)
		{
			mesh->DrawTriangle(shader);
		}
	}
}
