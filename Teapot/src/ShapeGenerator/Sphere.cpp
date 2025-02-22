#include "Sphere.h"

namespace Shapes
{
	Sphere::Sphere(const float radius, const glm::vec3 color, const int sectorCount, const int stackCount)
		: m_radius(radius)
		, m_color(color)
		, m_sectorCount(sectorCount)
        , m_stackCount(stackCount)
	{
        BuildVertexData();
	}

    void Sphere::BuildVertexData()
    {
        // vertex position
        float x  = 0;
        float y  = 0;
        float z  = 0;
        float xy = 0;

        // vertex normal
        float nx = 0;
        float ny = 0;
        float nz = 0;
        float lengthInv = 1.0f / m_radius;

        const float PI = acos(-1);

        float sectorStep = 2 * PI / m_sectorCount;
        float stackStep = PI / m_stackCount;
        float sectorAngle = 0;
        float stackAngle = 0;

        for (int i = 0; i <= m_stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;          // starting from pi/2 to -pi/2
            xy = m_radius * cosf(stackAngle);             // r * cos(u)
            z = m_radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= m_sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                Vertex tmp{};
                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;

                tmp.position.x = x;
                tmp.position.y = y;
                tmp.position.z = z;

                tmp.color = m_color;
                tmp.normal = glm::vec3(nx, ny, nz);

                vertices.push_back(tmp);
                positions.push_back(glm::vec3(x, y, z));
                colors.push_back(m_color);
                normals.push_back(glm::vec3(nx, ny, nz));
            }
        }

        // generate CCW m_dataIndex list of sphere triangles
        // k1--k1+1
        // |  / |
        // | /  |
        // k2--k2+1

        int k1 = 0;
        int k2 = 0;

        for (int i = 0; i < m_stackCount; ++i)
        {
            k1 = i * (m_sectorCount + 1);     // beginning of current stack
            k2 = k1 + m_sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < m_sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (m_stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }
    }
}