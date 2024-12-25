#include "Plane.h"

namespace Shapes
{
	Plane::Plane(const int width, const int height, const float sliceSize, const glm::vec3 color)
		: m_width(width)
		, m_height(height)
		, m_sliceSize(sliceSize)
		, m_color(color)
	{
		BuildVertices();
		BuildIndices();
	}

	void Plane::BuildVertices()
	{
		m_unitVertices.push_back({ m_sliceSize,		m_sliceSize,	0.0f });
		m_unitVertices.push_back({ 0.0f,			m_sliceSize,	0.0f });
		m_unitVertices.push_back({ 0.0f,			0.0f,			0.0f });
		m_unitVertices.push_back({ m_sliceSize,		0.0f,			0.0f });

		int rowK = 0;
		int colK = 0;

		for (int row = 0; row < m_width; row++)
		{
			rowK = row * m_sliceSize;
			for (int col = 0; col < m_height; col++)
			{
				colK = col * m_sliceSize;
				for (int i = 0; i < 4; i++)
				{
					vertices.emplace_back(
						glm::vec3{(m_unitVertices[i].x + colK), (m_unitVertices[i].y + rowK), m_unitVertices[i].z},
						m_color,
						glm::vec3(0, 0, 1)
					);

					positions.push_back(glm::vec3{ (m_unitVertices[i].x + colK), (m_unitVertices[i].y + rowK), m_unitVertices[i].z });
					colors.push_back(m_color);
					normals.push_back(glm::vec3(0, 0, 1));
				}
			}
		}
	}

	void Plane::BuildIndices()
	{
		int ind1 = 0;
		int ind2 = 1;
		int ind3 = 2;
		int ind4 = 3;

		int k = 0;

		for (int row = 0; row < m_width; row++)
		{
			for (int col = 0; col < m_height; col++)
			{
				indices.push_back(ind1 + k);
				indices.push_back(ind3 + k);
				indices.push_back(ind2 + k);

				indices.push_back(ind1 + k);
				indices.push_back(ind4 + k);
				indices.push_back(ind3 + k);

				k += 4;
			}
		}
	}
}
