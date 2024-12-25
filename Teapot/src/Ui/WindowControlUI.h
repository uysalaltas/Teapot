#pragma once
#include <imgui.h>

#include "Teapot/Window.h"
#include "Renderer/Model.h"
#include <ImGuizmo.h>

namespace Teapot
{
	class WindowControlUI
	{
	public:
		explicit WindowControlUI(Window& window) :m_window(window) {};

		void UIGizmos()
		{
			ImGui::Begin("Control", nullptr, 0);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Checkbox("Activate Gizmo", &m_window.IsGizmoActive);

			ImGui::RadioButton("Translate", &m_window.SelectedGizmo, ImGuizmo::OPERATION::TRANSLATE); ImGui::SameLine();
			ImGui::RadioButton("Scale"    , &m_window.SelectedGizmo, ImGuizmo::OPERATION::SCALE); ImGui::SameLine();
			ImGui::RadioButton("Rotate"   , &m_window.SelectedGizmo, ImGuizmo::OPERATION::ROTATE);

			if(ModelManager::GetModelVectorSize() == 0)
			{
				ImGui::End();
				return;
			}

			const char* combo_preview_value = ModelManager::GetSelectedModel()->name.c_str();
			if (ImGui::BeginCombo("Objects", combo_preview_value))
			{
				for (int n = 0; n < ModelManager::GetModelVectorSize(); n++)
				{
					const bool is_selected = (ModelManager::GetSelectedModelIndex() == n);
					if (ImGui::Selectable(ModelManager::GetModel(n)->name.c_str(), is_selected)) { ModelManager::SetSelectedModelIndex(n); }
					if (is_selected){ ImGui::SetItemDefaultFocus(); }
				}
				ImGui::EndCombo();
			}

			ImGui::InputFloat3("Object Translation", &ModelManager::GetSelectedModel()->objTranslation[0]);
			ImGui::InputFloat3("Object Scale", &ModelManager::GetSelectedModel()->objScale[0]);
			ImGui::InputFloat3("Object Rotation", &ModelManager::GetSelectedModel()->objRotation[0]);

			if (ImGui::Button("Manipulate Object")){ ModelManager::GetSelectedModel()->Manipulate();	}
			ImGui::End();
		}

	private:
		Window& m_window;
	};
}