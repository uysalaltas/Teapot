#pragma once

#include <iostream>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include "Utils.h"

namespace Shapes
{
	enum class ShapeObjects
	{
		NoShape = 0,
		Cube = 1,
		Cylinder = 2,
		Plane  = 3,
		Sphere = 4
	};

	struct Shape
	{
		std::vector<Teapot::Vertex> vertices = {};
		std::vector<GLuint> indices = {};
		std::vector<glm::vec3> positions = {};
		std::vector<glm::vec3> colors = {};
		std::vector<glm::vec3> normals = {};
	};

	class ShapeInterface : public Shape
	{
	public:
		virtual ~ShapeInterface() = default;
		virtual std::vector<GLuint>& ShapeIndices() = 0;
		virtual std::vector<Teapot::Vertex>& ShapeVertices() = 0;

		virtual std::vector<glm::vec3>& ShapePositions() = 0;
		virtual std::vector<glm::vec3>& ShapeColors() = 0;
		virtual std::vector<glm::vec3>& ShapeNormals() = 0;

		void PushData(Teapot::Vertex& v1, Teapot::Vertex& v2, Teapot::Vertex& v3, Teapot::Vertex& v4)
		{
			glm::vec3 n = m_utils.ComputeFaceNormals(v1, v3, v2);
			v1.normal = n;
			v2.normal = n;
			v3.normal = n;
			v4.normal = n;

			vertices.push_back(v1);
			vertices.push_back(v2);
			vertices.push_back(v3);
			vertices.push_back(v4);

			positions.push_back(v1.position);
			positions.push_back(v2.position);
			positions.push_back(v3.position);
			positions.push_back(v4.position);

			colors.push_back(v1.color);
			colors.push_back(v2.color);
			colors.push_back(v3.color);
			colors.push_back(v4.color);

			normals.push_back(v1.normal);
			normals.push_back(v2.normal);
			normals.push_back(v3.normal);
			normals.push_back(v4.normal);

			indices.push_back(m_dataIndex);
			indices.push_back(m_dataIndex + 2);
			indices.push_back(m_dataIndex + 1);

			indices.push_back(m_dataIndex + 1);
			indices.push_back(m_dataIndex + 2);
			indices.push_back(m_dataIndex + 3);
			m_dataIndex += 4;
		}

	private:
		Utils m_utils{};
		int m_dataIndex = 0;
	};
}

