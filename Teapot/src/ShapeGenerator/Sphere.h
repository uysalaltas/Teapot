#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Sphere : public ShapeInterface
	{
	public:
		Sphere(const float radius, const glm::vec3 color, const int sectorCount, const int sectorStep);
		inline std::vector<GLuint>& ShapeIndices() override { return indices; }
		inline std::vector<Teapot::Vertex>& ShapeVertices() override { return vertices; }
		inline std::vector<glm::vec3>& ShapePositions() override { return positions; }
		inline std::vector<glm::vec3>& ShapeColors() override { return colors; }
		inline std::vector<glm::vec3>& ShapeNormals() override { return normals; }

	private:
		void BuildVertexData();

	private:
		const float m_radius;
		const glm::vec3 m_color;
		const int m_sectorCount;
		const int m_stackCount;

		std::vector<glm::vec3> m_unitVertices;
	};
}