#include "ModelReader.h"
#include "ShapeGenerator/Shapes.h"

namespace Teapot
{
	ModelReader::ModelReader(std::shared_ptr<Teapot::ModelHandler> modelHandler
							,std::shared_ptr<Teapot::DebugModelHandler> debugModelHandler)
		: mp_modelHandler(modelHandler)
		, mp_debugModelHandler(debugModelHandler)
	{
	}

	bool ModelReader::CreateSceneFromXML(const std::string& xmlPath)
	{
		pugi::xml_document m_doc;

		if (pugi::xml_parse_result result = m_doc.load_file(xmlPath.c_str()); !result)
		{
			std::cout << "No xml" << std::endl;
			return false;
		}

		for (pugi::xml_node object : m_doc.child("objects"))
		{
			auto modelType = static_cast<Teapot::ModelType>(object.attribute("type").as_int());
			auto shapeType = static_cast<Shapes::ShapeObjects>(object.attribute("shape").as_int());
			auto pos = StringToVec3(object.attribute("pos").as_string());
			auto color = StringToVec3(object.attribute("color").as_string());
			auto scale = StringToVec3(object.attribute("scale").as_string());
			auto rot = StringToVec3(object.attribute("rot").as_string());
			auto alias = object.attribute("alias").as_string();

			std::shared_ptr<Teapot::ModelInterface> modelInt;
			std::shared_ptr<Teapot::ModelHandlerInterface> modelHandlerInterface;
			
			switch (modelType)
			{
				using enum Teapot::ModelType;
				case model: modelHandlerInterface = mp_modelHandler; break;
				case debugModel: modelHandlerInterface = mp_debugModelHandler; break;
				default: break;
			}

			if (!modelHandlerInterface)
			{
				std::cerr << "Invalid model type: " << static_cast<int>(modelType) << " for object: " << alias << std::endl;
				continue;
			}

			switch (shapeType)
			{
				using enum Shapes::ShapeObjects;
				case NoShape:
				{
					modelInt = modelHandlerInterface->CreateModel(object.attribute("path").as_string(), alias); 
					break;
				}
				case Cube:    
				{
					Shapes::Cube cube(1.0f, color);
					modelInt = modelHandlerInterface->CreateModel(cube, alias);
					break;
				}
				case Cylinder:
				{
					Shapes::Cylinder cylinder(1.0f, color, 30, 30);
					modelInt = modelHandlerInterface->CreateModel(cylinder, alias);
					break;
				}
				case Plane:
				{
					Shapes::Plane plane(30, 30, 1.0f, color); 
					modelInt = modelHandlerInterface->CreateModel(plane, alias);
					break;
				}
				case Sphere:
				{
					Shapes::Sphere sphere(1.0f, color, 30, 30);
					modelInt = modelHandlerInterface->CreateModel(sphere, alias);
					break;
				}
			}

			if (modelInt)
			{
				modelInt->Translate(pos);
				modelInt->Rotate(rot);
				modelInt->Scale(scale);
			}
		}

		return true;
	}

	void ModelReader::SaveSceneToXML(const std::string& xmlPath) const
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

		for (std::shared_ptr<ModelInterface> model : Teapot::ModelManager::GetModels())
		{
			auto modelType = static_cast<unsigned int>(model->modelType);
			pugi::xml_node object = objects.append_child("object");
			object.append_attribute("type") = modelType;
			object.append_attribute("alias") = model->name.c_str();
			object.append_attribute("pos") = Vec3ToString(model->objTranslation).c_str();
			object.append_attribute("color") = Vec3ToString(model->modelColor).c_str();
			object.append_attribute("scale") = Vec3ToString(model->objScale).c_str();
			object.append_attribute("rot") = Vec3ToString(model->objRotation).c_str();

			if (model->path != "")
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

	glm::vec3 ModelReader::StringToVec3(const std::string& str) const
	{
		try
		{
			std::string trimmed = str;
			trimmed.erase(trimmed.begin(), std::ranges::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) { return !std::isspace(ch); }));
			trimmed.erase(std::ranges::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) {	return !std::isspace(ch); }).base(), trimmed.end());
			std::ranges::replace(trimmed.begin(), trimmed.end(), ',', ' ');
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

	std::string ModelReader::Vec3ToString(const glm::vec3& vec) const
	{
		std::string result = std::format("{}, {}, {}", vec.x, vec.y, vec.z);
		return result;
	}
}
