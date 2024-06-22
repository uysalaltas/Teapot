#include "Cube.h"
#include "Utils.h"

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
		std::vector<Vertex> tmp_vertices = {};

		for (int i = 0; i <= 1; ++i)
		{
			float h = -1 + (float)i;
			for (int j = 0, k = 0; j <= 4; ++j, k += 3)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;

				Vertex tmp;
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
		Vertex v1{};
		Vertex v2{};
		Vertex v3{};
		Vertex v4{};
		glm::vec3 n{};

		int vi1 = 0;
		int vi2 = 0;
		int index = 0;

		for (int i = 0; i < 1; ++i)
		{
			vi1 = i * (4 + 1);
			vi2 = (i + 1) * (4 + 1);

			for (int j = 0; j < 4; ++j, ++vi1, ++vi2)
			{
				v1 = tmp_vertices[vi1];
				v2 = tmp_vertices[vi2];
				v3 = tmp_vertices[vi1 + 1];
				v4 = tmp_vertices[vi2 + 1];

				n = Utils::ComputeFaceNormals(v1, v3, v2);

				v1.normal = n;
				v2.normal = n;
				v3.normal = n;
				v4.normal = n;

				m_vertices.push_back(v1);
				m_vertices.push_back(v2);
				m_vertices.push_back(v3);
				m_vertices.push_back(v4);

				m_positions.push_back(v1.position);
				m_positions.push_back(v2.position);
				m_positions.push_back(v3.position);
				m_positions.push_back(v4.position);

				m_colors.push_back(v1.color);
				m_colors.push_back(v2.color);
				m_colors.push_back(v3.color);
				m_colors.push_back(v4.color);

				m_normals.push_back(v1.normal);
				m_normals.push_back(v2.normal);
				m_normals.push_back(v3.normal);
				m_normals.push_back(v4.normal);

				m_indices.push_back(index);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 1);

				m_indices.push_back(index + 1);
				m_indices.push_back(index + 2);
				m_indices.push_back(index + 3);
				index += 4;
			}
		}
	
		//------------------------
		//BASE AND TOP
		//------------------------
		unsigned int m_baseIndex = (int)m_vertices.size();
		unsigned int m_topIndex = m_baseIndex + 4;

		for (int i = 0; i < 2; i++)
		{
			float h = -1 + (float)i;
			float nz = -1 + i * 2;

			for (int j = 0; j < 4; j++)
			{
				float ux = m_unitVertices[j].x;
				float uy = m_unitVertices[j].y;
				
				Vertex tmp;
				tmp.position.x = ux * m_size;
				tmp.position.y = uy * m_size;
				tmp.position.z = h * m_size;
				tmp.color = m_color;
				tmp.normal.x = 0;
				tmp.normal.y = 0;
				tmp.normal.z = nz;

				m_vertices.push_back(tmp);
				m_positions.push_back(tmp.position);
				m_colors.push_back(tmp.color);
				m_normals.push_back(tmp.normal);
			}
		}

		// indices for the base surface
		for (int i = 0, k = m_baseIndex; i < 2; ++i, ++k)
		{
			m_indices.push_back(m_baseIndex);
			m_indices.push_back(k + 2);
			m_indices.push_back(k + 1);
		}

		// indices for the top surface
		for (int i = 0, k = m_topIndex; i < 2; ++i, ++k)
		{
			m_indices.push_back(m_topIndex);
			m_indices.push_back(k + 2);
			m_indices.push_back(k + 1);
		}
	}

}
