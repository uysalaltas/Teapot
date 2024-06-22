#pragma once

#include "Shapes.h"

namespace Shapes
{
	class Utils
	{
	public:
		static glm::vec3 ComputeFaceNormals(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	private:
		static Utils* s_Instance;
    };
}