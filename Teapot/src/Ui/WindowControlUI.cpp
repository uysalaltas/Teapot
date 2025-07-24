#include "WindowControlUI.h"

namespace Teapot
{
	void WindowControlUI::UIGizmos() const
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Activate Gizmo", &Teapot::SceneContext::Get().IsGizmoActive());

		ImGui::RadioButton("Translate", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
		ImGui::RadioButton("Scale", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::SCALE); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &Teapot::SceneContext::Get().GetSelectedGizmo(), ImGuizmo::OPERATION::ROTATE);

		if (ModelManager::GetModelVectorSize() == 0) { return; }

		if (ImGui::BeginCombo("Objects", ModelManager::GetSelectedModel()->name.c_str()))
		{
			for (const auto& model : ModelManager::GetModels())
			{
				const bool is_selected = (model == ModelManager::GetSelectedModel());
				if (ImGui::Selectable(model->name.c_str(), is_selected)) { ModelManager::SetSelectedModel(model); }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}

			ImGui::EndCombo();
		}

		ImGui::InputFloat3("Object Translation", &ModelManager::GetSelectedModel()->objTranslation[0]);
		ImGui::InputFloat3("Object Scale", &ModelManager::GetSelectedModel()->objScale[0]);
		ImGui::InputFloat3("Object Rotation", &ModelManager::GetSelectedModel()->objRotation[0]);

		if (ImGui::Button("Manipulate Object")) { ModelManager::GetSelectedModel()->Manipulate(); }
	}

	void WindowControlUI::UIFocusToObject() const
	{
		static bool isFocusToObjChecked = false;
		ImGui::Checkbox("Focus Object", &isFocusToObjChecked);

		if (isFocusToObjChecked)
		{
			Teapot::SceneContext::Get().GetCamera().SetLookAt(Teapot::ModelManager::GetSelectedModel()->objTranslation);
		}
	}
}