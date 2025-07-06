#include "WindowControlUI.h"

namespace Teapot
{
	void WindowControlUI::UIGizmos() const
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Checkbox("Activate Gizmo", &Teapot::SceneContext::Get().IsGizmoActive);

		ImGui::RadioButton("Translate", &Teapot::SceneContext::Get().SelectedGizmo, ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
		ImGui::RadioButton("Scale", &Teapot::SceneContext::Get().SelectedGizmo, ImGuizmo::OPERATION::SCALE); ImGui::SameLine();
		ImGui::RadioButton("Rotate", &Teapot::SceneContext::Get().SelectedGizmo, ImGuizmo::OPERATION::ROTATE);

		if (ModelManager::GetModelVectorSize() == 0)
		{
			return;
		}

		if (ImGui::BeginCombo("Objects", ModelManager::GetSelectedModel()->name.c_str()))
		{
			for (int n = 0; n < ModelManager::GetModelVectorSize(); n++)
			{
				const bool is_selected = (ModelManager::GetSelectedModelIndex() == n);
				if (ImGui::Selectable(ModelManager::GetModel(n)->name.c_str(), is_selected)) { ModelManager::SetSelectedModelIndex(n); }
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