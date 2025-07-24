#pragma once
#include "Shapes.h"

namespace Shapes
{
	class Plane : public ShapeInterface
	{
	public:
		Plane(const int height, const int width, const float sliceSize, const glm::vec3 color);
		inline std::vector<GLuint>& ShapeIndices() override { return indices; }
		inline std::vector<Teapot::Vertex>& ShapeVertices() override { return vertices; }
		inline std::vector<glm::vec3>& ShapePositions() override { return positions; }
		inline std::vector<glm::vec3>& ShapeColors() override { return colors; }
		inline std::vector<glm::vec3>& ShapeNormals() override { return normals; }

	private:
		void BuildVertices();
		void BuildIndices();

	private:
		const int m_width;
		const int m_height;
		const float m_sliceSize;
		const glm::vec3 m_color;
		std::vector<glm::vec3> m_unitVertices;
	};
}