#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cylinder : public Shapes
	{
	public:
		Cylinder(
			const float size, 
			const glm::vec3 color, 
			const float height = 2.0f,
			const int sectorCount = 8,
			const float baseRadius = 1.0f,
			const float topRadius = 1.0f
		);

		inline std::vector<GLuint>& ShapeIndices() override { return m_indices; }
		inline std::vector<Vertex>& ShapeVertices() override { return m_vertices; }
		inline std::vector<glm::vec3>& ShapePositions() override { return m_positions; }
		inline std::vector<glm::vec3>& ShapeColors() override { return m_colors; }
		inline std::vector<glm::vec3>& ShapeNormals() override { return m_normals; }

	private:
		void BuildVertexData();
		void BuildUnitCircleVertices();

	private:
		const float m_size;
		const glm::vec3 m_color;
		const float m_height;
		const int m_sectorCount;
		const float m_baseRadius;
		const float m_topRadius;

		const int m_stackCount = 2;
		
		unsigned int m_baseIndex;
		unsigned int m_topIndex;

		std::vector<float> m_unitCircleVertices;
	};
}