#include "Teapot.h"
#include <array>

class Sandbox : public Teapot::Application
{
public:
	explicit Sandbox(Teapot::WindowProps& props) :
		Teapot::Application(props)
	{
		CreateModelsFromXML("TeapotApp/Objects.xml");

		dirLight.position  = glm::vec3(3.0f, 5.0f, 5.0f);
		spotLight.position = glm::vec3(0.0f, 3.0f, 0.0f);
		spotLight.ambient  = glm::vec3(0.15f);

		GetLight().CreateDirectionalLight(dirLight);
		GetLight().CreateSpotLight(spotLight);
		GetLight().ActivateShadow(true);

		GetUI().ActivateGizmos(true);
		GetWindow().ActivateSnap(0.05f, 0.05f, 0.05f);
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

		// Ui Stuff
		GetLight().UIModifySpotLight();
		GetLight().UIModifyDirectionLight();
		GetLight().UIRenderShadowMap();

		GetUI().UIBegin("Control");
		GetUI().UIGizmos();
		GetUI().UIFocusToObject();
		GetUI().UIShape(GetModelHandler(), GetDebugModelHandler());
		GetUI().UIEnd();

		//AddShape();

		//ImGui::ShowDemoWindow();
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