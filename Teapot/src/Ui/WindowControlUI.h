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

			const char* combo_preview_value = Model::s_Models[Model::s_SelectedModel]->name.c_str();  // Pass in the preview value visible before opening the combo (it could be anything)

			if (ImGui::BeginCombo("Objects", combo_preview_value))
			{
				for (int n = 0; n < Model::s_Models.size(); n++)
				{
					const bool is_selected = (Model::s_SelectedModel == n);
					if (ImGui::Selectable(Model::s_Models[n]->name.c_str(), is_selected))
						Model::s_SelectedModel = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::InputFloat3("Object Translation", &Model::s_Models[Model::s_SelectedModel]->objTranslation[0]);
			ImGui::InputFloat3("Object Scale"      , &Model::s_Models[Model::s_SelectedModel]->objScale[0]      );
			ImGui::InputFloat3("Object Rotation"   , &Model::s_Models[Model::s_SelectedModel]->objRotation[0]   );

			if (ImGui::Button("Manipulate Object"))
			{
				Model::s_Models[Model::s_SelectedModel]->Manipulate();
			}

			ImGui::End();
		}

	private:
		Window& m_window;
	};
}