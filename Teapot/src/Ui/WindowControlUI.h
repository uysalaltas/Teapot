#pragma once
#include <imgui.h>
#include <format>
#include <functional>

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
		inline void UICreateButton(const std::string& buttonName, const std::function<void()>& callback) 
		{ 
			ImGui::Dummy(ImVec2(0.0f, 10.0f));
			if (ImGui::Button(buttonName.c_str())) { callback(); } 
		}

		void UIGizmos() const;
		void UIFocusToObject() const;
		void UIShape(
			std::shared_ptr<Teapot::ModelHandlerInterface> handler1 = nullptr,
			std::shared_ptr<Teapot::ModelHandlerInterface> handler2 = nullptr);

    private:
        void CreateShapeButton(
            std::shared_ptr<Teapot::ModelHandlerInterface> handler,
            const Shapes::ShapeObjects& selectedType,
            const glm::vec3& shapeColor) const;
    };
}