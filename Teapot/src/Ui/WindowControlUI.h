#pragma once
#include <imgui.h>

#include "Teapot/Window.h"
#include "Models/ModelManager.h"
#include "Scene/SceneContext.h"
#include "ShapeGenerator/ShapeFactory.h"
#include "Models/ModelHandlerInterface.h"

#include <ImGuizmo.h>

namespace Teapot
{
	class WindowControlUI
	{
	public:
		WindowControlUI() = default;

		inline void ActivateGizmos(const bool activate) const { Teapot::SceneContext::Get().IsGizmoActive() = activate; }
		inline void UIBegin(const char* sectionName) const { ImGui::Begin(sectionName, nullptr, 0); }
		inline void UIEnd() const { ImGui::End(); }

		void UIGizmos() const;
		void UIFocusToObject() const;
		void UIShape(
			std::shared_ptr<Teapot::ModelHandlerInterface> handler1 = nullptr,
			std::shared_ptr<Teapot::ModelHandlerInterface> handler2 = nullptr);
    private:
        void CreateShapeButton(
            std::shared_ptr<Teapot::ModelHandlerInterface> handler,
            Shapes::ShapeObjects& selectedType,
            glm::vec3& shapeColor);
    };
}