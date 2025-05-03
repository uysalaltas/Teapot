#include "Teapot.h"
#include <array>

class Sandbox : public Teapot::Application
{
public:
	explicit Sandbox(const Teapot::WindowProps& props) :
		Teapot::Application(props)
	{
		Teapot::ModelReader::CreateSceneFromXML("TeapotApp/Objects.xml");
		dirLight.position  = glm::vec3(1.2f, 5.0f, 5.0f);
		spotLight.position = glm::vec3(0.0f, 0.0f, 3.0f);
		spotLight.ambient  = glm::vec3(0.15f);

		GetShaderManager().CreateDirectionalLight(dirLight);
		GetShaderManager().CreateSpotLight(spotLight);
		GetShaderManager().ActivateShadow(true);

		GetUI().ActivateGizmos(true);
	}

	void OnUpdateAwake() override
	{
		// This function is executed before OnUpdate()
		// pure vitrual, mandatory to override
	}

	void OnUpdate() override
	{
		GetCamera().UpdateProjMatrix();
		GetCamera().ActivateArcballCamera();
		GetCamera().ActivatePanCamera();
		GetCamera().ActivateZoomCamera();
		GetCamera().ActivateFreeMovement();

		RenderScene();

		// Ui Stuff
		GetShaderManager().UIModifySpotLight();
		GetShaderManager().UIModifyDirectionLight();
		GetShaderManager().UIRenderShadowMap();

		GetUI().UIBegin("Control");
		GetUI().UIGizmos();
		GetUI().UIFocusToObject();
		GetUI().UIEnd();

		AddShape();

		//ImGui::ShowDemoWindow();
	}

	void RenderScene() const
	{
		Teapot::ModelManager::DrawModels();
	}

	void AddShape()
	{
		std::array<const char*, 3> preshapes = { "Cube", "Sphere", "Cylinder"};
		static int preshapesIdx = 0;
		const char* comboShapeValue = preshapes[preshapesIdx];

		ImGui::Begin("Add Shape", nullptr, 0);
		static glm::vec3 shapeColor = { 1.0f, 0.5f, 0.0f };
		ImGui::ColorEdit3("Color", &shapeColor[0]);

		if (ImGui::BeginCombo("Shapes", comboShapeValue))
		{
			for (int n = 0; n < preshapes.size(); n++)
			{
				const bool is_selected = (preshapesIdx == n);
				if (ImGui::Selectable(preshapes[n], is_selected)) { preshapesIdx = n; }
				if (is_selected) { ImGui::SetItemDefaultFocus(); }
			}
			ImGui::EndCombo();
		}

		if (ImGui::Button("Add Shape"))
		{
			auto shapeType = Teapot::ShapeObjects::Custom;
			if (preshapes[preshapesIdx] == "Cube")         { selectedShape = std::make_unique<Shapes::Cube>(1.0f, shapeColor);                shapeType = Teapot::ShapeObjects::Cube;     }
			else if (preshapes[preshapesIdx] == "Sphere")  { selectedShape = std::make_unique<Shapes::Sphere>(0.30f, shapeColor, 30, 30);     shapeType = Teapot::ShapeObjects::Sphere;   }
			else if (preshapes[preshapesIdx] == "Cylinder"){ selectedShape = std::make_unique<Shapes::Cylinder>(0.30f, shapeColor, 1.0f, 30); shapeType = Teapot::ShapeObjects::Cylinder; }

			static unsigned int counter = 0;
			Teapot::Model::CreateModel(*selectedShape, std::format("Shape {}",counter), shapeType);
			counter++;
		}

		ImGui::SameLine(); if (ImGui::Button("Remove Shape"))	{ Teapot::Model::RemoveModel();	}
		ImGui::SameLine(); if (ImGui::Button("Save Scene")){ Teapot::ModelReader::SaveSceneToXML("TeapotApp/Objects.xml");	}
		ImGui::AlignTextToFramePadding();
		if (ImGui::Button("Focus Object")) { GetCamera().SetLookAt(Teapot::ModelManager::GetSelectedModel()->objTranslation); }

		ImGui::End();
	}

private:
	std::unique_ptr<Shapes::ShapeInterface> selectedShape;
	std::shared_ptr<Teapot::Model> sphrModel;

	float ambientStrength = 0.40f;
	float specularStrength = 0.35f;
	float shininess = 50.0f;

	Teapot::DirectionalLight dirLight{};
	Teapot::PointLight pointLight{};
	Teapot::SpotLight spotLight{};
};

int main()
{
	Teapot::WindowProps windowProps = { "Shape Demo", 1280, 720, glm::vec4{0.05f, 0.07f, 0.09f, 1.0f} };
	auto s = std::make_unique<Sandbox>(windowProps);
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
}