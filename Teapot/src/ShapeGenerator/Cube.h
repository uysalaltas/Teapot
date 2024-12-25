#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Cube : public ShapeInterface
	{
	public:
		Cube(const float size, const glm::vec3 color);
		inline std::vector<GLuint>& ShapeIndices() override { return indices; }
		inline std::vector<Vertex>& ShapeVertices() override { return vertices; }
		inline std::vector<glm::vec3>& ShapePositions() override { return positions; }
		inline std::vector<glm::vec3>& ShapeColors() override { return colors; }
		inline std::vector<glm::vec3>& ShapeNormals() override { return normals; }
		
	private:
		void BuildVertexData();

	private:
		const float m_size;
		const glm::vec3 m_color;

		std::vector<glm::vec3> m_unitVertices;
	};
}