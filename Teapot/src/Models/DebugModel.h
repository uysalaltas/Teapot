#pragma once

#include "ModelInterface.h"

namespace Teapot
{
	class DebugModel : public ModelInterface
	{
	public:
		DebugModel(const Shapes::Shape& shapes, const std::string& nameObject);
		void Draw(Teapot::Shader& shader) override;
	};
}