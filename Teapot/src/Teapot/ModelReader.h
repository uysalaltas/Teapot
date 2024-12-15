#pragma once
#include "pugixml.hpp"

#include "ShapeGenerator/Cube.h"
#include "ShapeGenerator/Cylinder.h"
#include "ShapeGenerator/Plane.h"
#include "ShapeGenerator/Sphere.h"

#include <string>
#include <sstream>

namespace Teapot
{
	class ModelReader
	{
	public:
		enum class ShapeObjects
		{
			PathObj = 0,
			Cube = 1,
			Cylinder = 2,
			Plane = 3,
			Sphere = 4
		};

		ModelReader() = default;

		bool CreateObjectFromXML(std::string xmlPath)
		{
			m_XMLPath = xmlPath;
			pugi::xml_parse_result result = m_doc.load_file(m_XMLPath.c_str());
			if (!result)
			{
				std::cout << "No xml" << std::endl;
				return false;
			}

			for (pugi::xml_node object : m_doc.child("objects"))
			{
				ShapeObjects type = static_cast<ShapeObjects>(object.attribute("type").as_int());
				auto pos = StringToVec3(object.attribute("pos").as_string());
				auto color = StringToVec3(object.attribute("color").as_string());
				auto scale = StringToVec3(object.attribute("scale").as_string());
				auto alias = object.attribute("alias").as_string();

				switch (type)
				{
				case ShapeObjects::PathObj:
				{
					auto path = object.attribute("path").as_string();
					auto model = Teapot::Model::CreateModel(path, alias);
					model->Translate(pos);
					model->Scale(scale);
					break;
				}
				case ShapeObjects::Cube:
				{
					Shapes::Cube obj(1.0f, color);
					auto model = Teapot::Model::CreateModel(obj.ShapePositions(), obj.ShapeColors(), obj.ShapeNormals(), obj.ShapeIndices(), alias);
					model->Translate(pos);
					model->Scale(scale);
					break;
				}
				case ShapeObjects::Sphere:
				{
					Shapes::Sphere obj(1.0f, color, 30, 30);
					auto model = Teapot::Model::CreateModel(obj.ShapePositions(), obj.ShapeColors(), obj.ShapeNormals(), obj.ShapeIndices(), alias);
					model->Translate(pos);
					model->Scale(scale);
					break;
				}
				}
			}

			return true;
		}

	private:
		glm::vec3 StringToVec3(const std::string& str)
		{
			std::string trimmed = str;
			trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) { return !std::isspace(ch); }));
			trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {	return !std::isspace(ch); }).base(), trimmed.end());

			std::replace(trimmed.begin(), trimmed.end(), ',', ' ');

			// Parse the string into a vec3
			std::istringstream iss(trimmed);
			float x, y, z;
			if (!(iss >> x >> y >> z)) {
				throw std::invalid_argument("Invalid input format. Expected format: 'x, y, z'");
			}

			return glm::vec3(x, y, z);
		}

	private:
		std::string m_XMLPath;
		pugi::xml_document m_doc;
	};
}