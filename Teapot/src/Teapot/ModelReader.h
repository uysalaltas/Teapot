#pragma once
#include "pugixml.hpp"

#include "ShapeGenerator/Cube.h"
#include "ShapeGenerator/Cylinder.h"
#include "ShapeGenerator/Plane.h"
#include "ShapeGenerator/Sphere.h"
#include "Models/ModelHandler.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

namespace Teapot
{
	class ModelReader
	{
	public:
		ModelReader(std::shared_ptr<Teapot::ModelHandler> modelHandler);

		bool CreateSceneFromXML(const std::string& xmlPath);
		void SaveSceneToXML(const std::string& xmlPath);

	private:
		glm::vec3 StringToVec3(const std::string& str);
		std::string Vec3ToString(const glm::vec3& vec);

		std::shared_ptr<Teapot::ModelHandler> m_modelHandler;
	};
}