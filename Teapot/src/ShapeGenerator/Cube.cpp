#include "Cube.h"

namespace Shapes
{
	Cube::Cube(const float size, const glm::vec3 color)
		: m_size(size)
		, m_color(color)
	{
		BuildVertexData();
	}

	void Cube::BuildVertexData()
	{
		m_unitVertices.push_back({  0.5f,  0.5f, 0.0f });
		m_unitVertices.push_back({ -0.5f,  0.5f, 0.0f });
		m_unitVertices.push_back({ -0.5f, -0.5f, 0.0f });
		m_unitVertices.push_back({  0.5f, -0.5f, 0.0f });
		m_unitVertices.push_back({  0.5f,  0.5f, 0.0f });

		// ------------------------
		// SIDE
		// ------------------------
		std::vector<Teapot::Vertex> tmp_vertices = {};

		for (int i = 0; i <= 1; ++i)
		{
			float h = -1 + (float)i;
			for (int j = 0, k = 0; j <= 4; ++j, k += 3)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;

				Teapot::Vertex tmp;
				tmp.position.x = ux * m_size;
				tmp.position.y = uy * m_size;
				tmp.position.z = h  * m_size;

				tmp.color = m_color;
				tmp_vertices.push_back(tmp);
			}
		}

		// ------------------------
		// FLAT NORMALS
		// ------------------------
		int vi1 = 0;
		int vi2 = 0;

		for (int i = 0; i < 1; ++i)
		{
			vi1 = i * (4 + 1);
			vi2 = (i + 1) * (4 + 1);

			for (int j = 0; j < 4; ++j, ++vi1, ++vi2)
			{
				PushData(tmp_vertices[vi1], tmp_vertices[vi2], tmp_vertices[vi1 + 1], tmp_vertices[vi2 + 1]);
			}
		}
	
		//------------------------
		//BASE AND TOP
		//------------------------
		unsigned int m_baseIndex = (int)vertices.size();
		unsigned int m_topIndex = m_baseIndex + 4;

		for (int i = 0; i < 2; i++)
		{
			float h = -1 + (float)i;
			float nz = -1 + i * 2;

			for (int j = 0; j < 4; j++)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;
				
				Teapot::Vertex tmp;
				tmp.position.x = ux * m_size;
				tmp.position.y = uy * m_size;
				tmp.position.z = h * m_size;
				tmp.color = m_color;
				tmp.normal.x = 0;
				tmp.normal.y = 0;
				tmp.normal.z = nz;

				vertices.push_back(tmp);
				positions.push_back(tmp.position);
				colors.push_back(tmp.color);
				normals.push_back(tmp.normal);
			}
		}

		// indices for the base surface
		for (int i = 0, k = m_baseIndex; i < 2; ++i, ++k)
		{
			indices.push_back(m_baseIndex);
			indices.push_back(k + 2);
			indices.push_back(k + 1);
		}

		// indices for the top surface
		for (int i = 0, k = m_topIndex; i < 2; ++i, ++k)
		{
			indices.push_back(m_topIndex);
			indices.push_back(k + 2);
			indices.push_back(k + 1);
		}
	}

}
