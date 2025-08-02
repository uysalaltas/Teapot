#pragma once
#include "pugixml.hpp"

#include "ShapeGenerator/Cube.h"
#include "ShapeGenerator/Cylinder.h"
#include "ShapeGenerator/Plane.h"
#include "ShapeGenerator/Sphere.h"
#include "Models/Model/ModelHandler.h"
#include "Models/DebugModel/DebugModelHandler.h"

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
		explicit ModelReader(std::shared_ptr<Teapot::ModelHandler> modelHandler
							,std::shared_ptr<Teapot::DebugModelHandler> debugModelHandler);

		bool CreateSceneFromXML(const std::string& xmlPath);
		void SaveSceneToXML(const std::string& xmlPath) const;

	private:
		glm::vec3 StringToVec3(const std::string& str) const;
		std::string Vec3ToString(const glm::vec3& vec) const;

		std::shared_ptr<Teapot::ModelHandler> mp_modelHandler;
		std::shared_ptr<Teapot::DebugModelHandler> mp_debugModelHandler;
	};
}