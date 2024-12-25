#pragma once
#include "Renderer/VertexBuffer.h"

namespace Shapes
{
	class Utils
	{
	public:
		Utils() = default;
		glm::vec3 ComputeFaceNormals(const Vertex& v1, const Vertex& v2, const Vertex& v3) const
		{
			const float EPSILON = 0.000001f;

			auto normal = glm::vec3(0.0f, 0.0f, 0.0f);
			float nx = 0;
			float ny = 0;
			float nz = 0;

			// find 2 edge vectors: v1-v2, v1-v3
			float ex1 = v2.position.x - v1.position.x;
			float ey1 = v2.position.y - v1.position.y;
			float ez1 = v2.position.z - v1.position.z;
			float ex2 = v3.position.x - v1.position.x;
			float ey2 = v3.position.y - v1.position.y;
			float ez2 = v3.position.z - v1.position.z;

			// cross product: e1 x e2
			nx = ey1 * ez2 - ez1 * ey2;
			ny = ez1 * ex2 - ex1 * ez2;
			nz = ex1 * ey2 - ey1 * ex2;

			// normalize only if the length is > 0
			if (auto length = sqrtf(nx * nx + ny * ny + nz * nz); length > EPSILON)
			{
				// normalize
				float lengthInv = length > EPSILON ? 1.0f / length : 0.0f;
				normal.x = nx * lengthInv;
				normal.y = ny * lengthInv;
				normal.z = nz * lengthInv;
			}

			return normal;
		}
    };
}