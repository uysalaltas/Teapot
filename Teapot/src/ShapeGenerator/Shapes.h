#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include "Utils.h"

namespace Shapes
{
	class Shapes
	{
	public:
		virtual ~Shapes() = default;
		virtual std::vector<GLuint>& ShapeIndices() = 0;
		virtual std::vector<Vertex>& ShapeVertices() = 0;

		virtual std::vector<glm::vec3>& ShapePositions() = 0;
		virtual std::vector<glm::vec3>& ShapeColors() = 0;
		virtual std::vector<glm::vec3>& ShapeNormals() = 0;

		void PushData(Vertex& v1, Vertex& v2, Vertex& v3, Vertex& v4)
		{
			glm::vec3 n = m_utils.ComputeFaceNormals(v1, v3, v2);
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

			m_indices.push_back(m_dataIndex);
			m_indices.push_back(m_dataIndex + 2);
			m_indices.push_back(m_dataIndex + 1);

			m_indices.push_back(m_dataIndex + 1);
			m_indices.push_back(m_dataIndex + 2);
			m_indices.push_back(m_dataIndex + 3);
			m_dataIndex += 4;
		}

	protected:
		std::vector<Vertex> m_vertices = {};
		std::vector<GLuint> m_indices = {};
		std::vector<glm::vec3> m_positions = {};
		std::vector<glm::vec3> m_colors = {};
		std::vector<glm::vec3> m_normals = {};

	private:
		Utils m_utils{};
		int m_dataIndex = 0;
	};
}

