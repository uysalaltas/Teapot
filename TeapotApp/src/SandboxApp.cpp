#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	explicit Sandbox(const Teapot::WindowProps& props = Teapot::WindowProps::WindowProps()) :
		Teapot::Application(props)
	{
		cubeModel = std::make_unique<Teapot::Model>(cube.ShapePositions(), cube.ShapeColors(), cube.ShapeNormals(), cube.ShapeIndices(), "Cube");
		planeModel = std::make_unique<Teapot::Model>(plane.ShapePositions(), plane.ShapeColors(), plane.ShapeNormals(), plane.ShapeIndices(), "Plane");
		pyramidModel = std::make_unique<Teapot::Model>(pyramid.ShapePositions(), pyramid.ShapeColors(), pyramid.ShapeNormals(), pyramid.ShapeIndices(), "Pyramid");
		cylinderModel = std::make_unique<Teapot::Model>(cylinder.ShapePositions(), cylinder.ShapeColors(), cylinder.ShapeNormals(), cylinder.ShapeIndices(), "Cylinder");
		sphereModel = std::make_unique<Teapot::Model>(sphere.ShapePositions(), sphere.ShapeColors(), sphere.ShapeNormals(), sphere.ShapeIndices(), "Sphere");

		teaCup = std::make_unique<Teapot::Model>("coffee_cup_obj.obj", "cup");

		cubeModel->Translate(glm::vec3(1.0f, 0.0f, 0.0f));
		planeModel->Translate(glm::vec3(-5.0f, -5.0f, -0.3f));
		pyramidModel->Translate(glm::vec3(2.0f, 0.0f, 0.0f));
		cylinderModel->Translate(glm::vec3(-1.0f, 0.0f, 0.0f));
		teaCup->Translate(glm::vec3(0.0f, -2.0f, 0.0f));
		teaCup->Rotate(glm::vec3(90.0f, 0.0f, 0.0f));
		Teapot::Model::s_SelectedModel = 0;

		shaderManager->CreateDirectionalLight(dirLight);
		//shaderManager->CreateDirectionalLight(dirLight);
		//shaderManager->CreateDirectionalLight(dirLight);
		//shaderManager->CreatePointLight(pointLight);
		//shaderManager->CreatePointLight(pointLight);
		//shaderManager->CreatePointLight(pointLight);
		//shaderManager->CreatePointLight(pointLight);
		//shaderManager->CreateSpotLight(spotLight);
		//shaderManager->CreateSpotLight(spotLight);
	}

	void OnUpdateAwake() override
	{
		// This function is executed before OnUpdate()
		// pure vitrual, mandatory to override
	}

	void OnUpdate() override
	{
		camera->UpdateProjMatrix();
		camera->CalculateArcballCamera();
		camera->CalculatePanCamera();
		camera->ZoomCamera();

		RenderScene();
		//cubeLightModel->Translate(shaderManager->GetLightPos());

		// Ui Stuff
		//shaderManager->UIModifySpotLight();
		//shaderManager->UIModifyPointLight();
		shaderManager->UIModifyDirectionLight();
		windowUI->UIGizmos();

		ImGui::ShowDemoWindow();
	}

	void RenderScene()
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		teaCup->Draw();

		cubeModel->Draw();
		planeModel->Draw();
		pyramidModel->Draw();
		cylinderModel->Draw();
		sphereModel->Draw();
	}

private:
	Shapes::Cylinder cylinder{ 0.30f, glm::vec3(1.0f, 0.15f, 0.50f) };
	Shapes::Plane    plane{ 10, 10, 1.0f, glm::vec3(0.20f, 0.15f, 0.15f) };
	Shapes::Cube     cube{ 0.30f, glm::vec3(1.0f, 0.87f, 0.0f) };
	Shapes::Cylinder pyramid{ 0.30f, glm::vec3(0.20f, 0.71f, 0.29f), 2.0f, 4, 1.0f, 0.0f };
	Shapes::Sphere   sphere{ 0.30f, glm::vec3(0.20f, 0.25f, 1.0f), 30, 30 };

	std::unique_ptr<Teapot::Model> cubeModel;
	std::unique_ptr<Teapot::Model> planeModel;
	std::unique_ptr<Teapot::Model> cylinderModel;
	std::unique_ptr<Teapot::Model> pyramidModel;
	std::unique_ptr<Teapot::Model> sphereModel;
	std::unique_ptr<Teapot::Model> teaCup;

	float ambientStrength = 0.40f;
	float specularStrength = 0.35f;
	float shininess = 50.0f;

	Teapot::DirectionalLight dirLight{};
	Teapot::PointLight pointLight{};
	Teapot::SpotLight spotLight{};
};

int main()
{
	Teapot::WindowProps windowProps = { "Shape Demo", 1280, 720 };
	auto s = std::make_unique<Sandbox>(windowProps);
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
}