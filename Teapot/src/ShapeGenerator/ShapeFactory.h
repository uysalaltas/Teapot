#pragma once
#include "Shapes.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Plane.h"

#include <functional>
#include <map>

namespace Shapes
{
    struct ShapeFactoryInstance
    {
        const char* name;
        std::function<Shapes::Shape(const glm::vec3& color)> createFunc;
    };

    inline static std::map<ShapeObjects, ShapeFactoryInstance> shapeCreationMap
    {
        { ShapeObjects::Cube,     { "Cube",     [](const glm::vec3& color) { return Shapes::Cube(1.0f, color); } } },
        { ShapeObjects::Sphere,   { "Sphere",   [](const glm::vec3& color) { return Shapes::Sphere(0.30f, color, 30, 30); } } },
        { ShapeObjects::Cylinder, { "Cylinder", [](const glm::vec3& color) { return Shapes::Cylinder(0.30f, color, 1.0f, 30); } } }
        //{ ShapeObjects::Plane,    { "Plane",    [](const glm::vec3& color) { return std::make_unique<Shapes::Plane>(); } } }
    };
}