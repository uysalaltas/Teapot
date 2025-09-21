#include "WindowControlUI.h"

namespace Teapot
{
	void WindowControlUI::UIGizmos() const
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Activate Gizmo", &Teapot::SceneContext::Get().IsGizmoActive());

		ImGui::RadioButton("Translate", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
		ImGui::RadioButton("Scale", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::SCALE); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::ROTATE);

		if (Teapot::ModelManager::GetModelVectorSize() == 0) { return; }

		if (ImGui::BeginCombo("Objects", Teapot::ModelManager::GetSelectedModel()->name.c_str()))
		{
			for (const auto& model : Teapot::ModelManager::GetModels())
			{
				const bool is_selected = (model == Teapot::ModelManager::GetSelectedModel());
				if (ImGui::Selectable(model->name.c_str(), is_selected)) { Teapot::ModelManager::SetSelectedModel(model); }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}

			ImGui::EndCombo();
		}

		ImGui::InputFloat3("Object Translation", &Teapot::ModelManager::GetSelectedModel()->objTranslation[0]);
		ImGui::InputFloat3("Object Scale", &Teapot::ModelManager::GetSelectedModel()->objScale[0]);
		ImGui::InputFloat3("Object Rotation", &Teapot::ModelManager::GetSelectedModel()->objRotation[0]);

		if (ImGui::Button("Manipulate Object")) { Teapot::ModelManager::GetSelectedModel()->Manipulate(); }
	}

	void WindowControlUI::UIFocusToObject() const
	{
		ImGui::Dummy(ImVec2(0.0f, 10.0f));

		if (ImGui::Button("Focus Object") && Teapot::ModelManager::GetSelectedModel())
		{
			Teapot::SceneContext::Get().GetCamera().SetLookAt(Teapot::ModelManager::GetSelectedModel()->objTranslation);
		}
	}

	void WindowControlUI::UIShape(
		std::shared_ptr<Teapot::ModelHandlerInterface> handler1,
		std::shared_ptr<Teapot::ModelHandlerInterface> handler2) const
	{
		if (!handler1 && !handler2) return;

		static Shapes::ShapeObjects selectedType = Shapes::ShapeObjects::Cube;
		static glm::vec3 shapeColor = { 1.0f, 0.5f, 0.0f };

		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::ColorEdit3("Color", &shapeColor[0]);

		if (ImGui::BeginCombo("Shapes", Shapes::shapeCreationMap.at(selectedType).name))
		{
			for (const auto& [key, value] : Shapes::shapeCreationMap)
			{
				const bool isSelected = (selectedType == key);
				if (ImGui::Selectable(value.name, isSelected))
				{
					selectedType = key;
				}

				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		if (handler1)
		{
			CreateShapeButton(handler1, selectedType, shapeColor);
		}
		
		if (handler2)
		{
			CreateShapeButton(handler2, selectedType, shapeColor);
		}

		ImGui::SameLine();
		if (ImGui::Button("Remove Shape")) { Teapot::ModelManager::RemoveSelectedModel(); }
	}

    void WindowControlUI::CreateShapeButton(
		std::shared_ptr<Teapot::ModelHandlerInterface> handler,
		const Shapes::ShapeObjects& selectedType,
		const glm::vec3& shapeColor) const
	{
		static unsigned int counter = 0;
		std::string buttonLabel = std::format("Create {} for {}", Shapes::shapeCreationMap.at(selectedType).name, handler->modelHandlerName);
		if (ImGui::Button(buttonLabel.c_str()))
		{
			auto it = Shapes::shapeCreationMap.find(selectedType);
			if (it != Shapes::shapeCreationMap.end())
			{
				auto shapePtr = it->second.createFunc(shapeColor);

				// Call CreateModel for each handler
				handler->CreateModel(shapePtr, std::format("Shape {}", counter));
				counter++;
			}
		}
	}
}