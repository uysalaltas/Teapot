#include "Cylinder.h"

namespace Shapes
{
	Cylinder::Cylinder(
		const float size, 
		const glm::vec3 color, 
		const float height, 
		const int sectorCount, 
		const float baseRadius, 
		const float topRadius
	)
		: m_size(size)
		, m_color(color)
		, m_height(height)
		, m_sectorCount(sectorCount)
		, m_baseRadius(baseRadius)
		, m_topRadius(topRadius)
	{
		BuildUnitCircleVertices();
		BuildVertexData();
	}

	void Cylinder::BuildVertexData()
	{
		// ------------------------
		// SIDE
		// ------------------------
		std::vector<Teapot::Vertex> tmp_vertices = {};

		for (int i = 0; i <= m_stackCount; ++i)
		{
			float h = -(m_height / 2.0f) + (float)i / m_stackCount * m_height;			// z value; -h/2 to h/2
			float radius = m_baseRadius + (float)i / m_stackCount * (m_topRadius - m_baseRadius);

			for (int j = 0, k = 0; j <= m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];
				
				Teapot::Vertex tmp;
				tmp.position.x = ux * radius * m_size;
				tmp.position.y = uy * radius * m_size;
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

		// v2-v4 <== stack at i+1
		// | \ |
		// v1-v3 <== stack at i

		for (int i = 0; i < m_stackCount; ++i)
		{
			vi1 = i * (m_sectorCount + 1);
			vi2 = (i + 1) * (m_sectorCount + 1);

			for (int j = 0; j < m_sectorCount; ++j, ++vi1, ++vi2)
			{
				PushData(tmp_vertices[vi1], tmp_vertices[vi2], tmp_vertices[vi1 + 1], tmp_vertices[vi2 + 1]);
			}
		}

		//------------------------
		//BASE AND TOP
		//------------------------
		m_baseIndex = (int)vertices.size();
		m_topIndex = m_baseIndex + m_sectorCount + 1;

		for (int i = 0; i < 2; ++i)
		{
			float h = -m_height / 2.0f + i * m_height;			// z value; -h/2 to h/2
			float nz = -1 + i * 2;								// z value of normal; -1 to 1

			Teapot::Vertex centerVertex;
			centerVertex.position.x = 0.0f;
			centerVertex.position.y = 0.0f;
			centerVertex.position.z = h * m_size;

			centerVertex.color = m_color;

			centerVertex.normal.x = 0;
			centerVertex.normal.y = 0;
			centerVertex.normal.z = nz;
			
			vertices.push_back(centerVertex);
			positions.push_back(centerVertex.position);
			colors.push_back(centerVertex.color);
			normals.push_back(centerVertex.normal);

			for (int j = 0, k = 0; j < m_sectorCount; ++j, k += 3)
			{
				float ux = m_unitCircleVertices[k];
				float uy = m_unitCircleVertices[k + 1];

				Teapot::Vertex tmp;
				tmp.position.x = ux * m_topRadius * m_size;
				tmp.position.y = uy * m_topRadius * m_size;
				tmp.position.z = h  * m_size;

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
		for (int i = 0, k = m_baseIndex + 1; i < m_sectorCount; ++i, ++k)
		{
			if (i < m_sectorCount - 1)
			{
				indices.push_back(m_baseIndex);
				indices.push_back(k + 1);
				indices.push_back(k);
			}
			else // last triangle
			{
				indices.push_back(m_baseIndex);
				indices.push_back(m_baseIndex + 1);
				indices.push_back(k);
			}
		}

		// indices for the top surface
		for (int i = 0, k = m_topIndex + 1; i < m_sectorCount; ++i, ++k)
		{
			if (i < m_sectorCount - 1)
			{
				indices.push_back(m_topIndex);
				indices.push_back(k);
				indices.push_back(k + 1);
			}
			else // last triangle
			{
				indices.push_back(m_topIndex);
				indices.push_back(k);
				indices.push_back(m_topIndex + 1);
			}
		}
	}

	void Cylinder::BuildUnitCircleVertices()
	{
		const float PI = acos(-1);
		float sectorStep = 2 * PI / m_sectorCount;
		float sectorAngle;  // radian

		for (int i = 0; i <= m_sectorCount; ++i)
		{
			sectorAngle = i * sectorStep;
			m_unitCircleVertices.push_back(cos(sectorAngle)); // x
			m_unitCircleVertices.push_back(sin(sectorAngle)); // y
			m_unitCircleVertices.push_back(0);                // z
		}
	}
}
