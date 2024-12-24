#pragma once
#include "pugixml.hpp"

#include "ShapeGenerator/Cube.h"
#include "ShapeGenerator/Cylinder.h"
#include "ShapeGenerator/Plane.h"
#include "ShapeGenerator/Sphere.h"
#include "Renderer/Model.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace Teapot
{
	class ModelReader
	{
	public:
		ModelReader() = default;

		static inline bool CreateSceneFromXML(const std::string& xmlPath)
		{
			pugi::xml_document m_doc;

			if (pugi::xml_parse_result result = m_doc.load_file(xmlPath.c_str()); !result)
			{
				std::cout << "No xml" << std::endl;
				return false;
			}

			for (pugi::xml_node object : m_doc.child("objects"))
			{
				auto type = static_cast<ShapeObjects>(object.attribute("type").as_int());
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
						auto model = Teapot::Model::CreateModel(Shapes::Cube(1.0f, color), alias, ShapeObjects::Cube);
						model->Translate(pos);
						model->Scale(scale);
						break;
					}
					case ShapeObjects::Sphere:
					{
						Shapes::Sphere obj(1.0f, color, 30, 30);
						auto model = Teapot::Model::CreateModel(Shapes::Sphere(1.0f, color, 30, 30), alias, ShapeObjects::Sphere);
						model->Translate(pos);
						model->Scale(scale);
						break;
					}
				}
			}

			return true;
		}

		static inline void SaveSceneToXML(const std::string& xmlPath)
		{
			// File exists, clear its content
			if (std::ifstream fileCheck(xmlPath); fileCheck.is_open())
			{
				fileCheck.close();
				std::ofstream clearFile(xmlPath, std::ios::trunc);
				if (clearFile.is_open()) 
				{
					clearFile.close();
					std::cout << "Existing XML file cleared.\n";
				}
				else 
				{
					std::cerr << "Failed to clear the XML file.\n";
					return;
				}
			}

			pugi::xml_document doc;

			// Add root node
			pugi::xml_node objects = doc.append_child("objects");

			for (std::shared_ptr<Model> model : ModelManager::GetModelVector())
			{
				auto shapeType = static_cast<unsigned int>(model->shapeType);
				pugi::xml_node object = objects.append_child("object");
				object.append_attribute("type") = shapeType;
				object.append_attribute("alias") = model->name.c_str();
				object.append_attribute("pos") = Vec3ToString(model->objTranslation).c_str();
				object.append_attribute("color") = Vec3ToString(model->modelColor).c_str();
				object.append_attribute("scale") = Vec3ToString(model->objScale).c_str();
				object.append_attribute("rot") = Vec3ToString(model->objRotation).c_str();
				
				if (shapeType == static_cast<unsigned int>(ShapeObjects::PathObj))
				{
					object.append_attribute("path") = model->path.c_str();
				}
			}

			// Save to file
			if (doc.save_file(xmlPath.c_str())) 
			{
				std::cout << "XML document created successfully.\n";
			}
			else 
			{
				std::cerr << "Failed to create XML document.\n";
			}
		}

	private:
		static inline glm::vec3 StringToVec3(const std::string& str)
		{
			try
			{
				std::string trimmed = str;
				trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) { return !std::isspace(ch); }));
				trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {	return !std::isspace(ch); }).base(), trimmed.end());
				std::replace(trimmed.begin(), trimmed.end(), ',', ' ');
				std::istringstream iss(trimmed);
				float x, y, z;
				if (!(iss >> x >> y >> z)) {
					throw std::invalid_argument("Invalid input format. Expected format: 'x, y, z'");
				}

				return glm::vec3(x, y, z);
			}
			catch (const std::invalid_argument& e)
			{
				std::cerr << "Error parsing vector string '" << str << "': " << e.what() << std::endl;
				return glm::vec3(0.0f);
			}
		}
		
		static inline std::string Vec3ToString(const glm::vec3& vec)
		{
			std::string result;

			result = std::to_string(vec.x) + ", " + 
					 std::to_string(vec.y) + ", " +
					 std::to_string(vec.z);

			return result;
		}

	};
}