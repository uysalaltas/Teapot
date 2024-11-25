#include "Teapot.h"

class Sandbox : public Teapot::Application
{
public:
	explicit Sandbox(const Teapot::WindowProps& props) :
		Teapot::Application(props)
	{
		cubeBase  = Teapot::Model::CreateModel(cube1.ShapePositions() , cube1.ShapeColors() , cube1.ShapeNormals() , cube1.ShapeIndices() , "CubeBase" );
		cubeLeft  = Teapot::Model::CreateModel(cube2.ShapePositions() , cube2.ShapeColors() , cube2.ShapeNormals() , cube2.ShapeIndices() , "CubeLeft" );
		cubeRight = Teapot::Model::CreateModel(cube3.ShapePositions() , cube3.ShapeColors() , cube3.ShapeNormals() , cube3.ShapeIndices() , "CubeRight");
		sphrModel = Teapot::Model::CreateModel(sphere.ShapePositions(), sphere.ShapeColors(), sphere.ShapeNormals(), sphere.ShapeIndices(), "Sphere"   );

		cubeBase ->Translate(glm::vec3( 0.00f,  0.00f, 0.00f));
		cubeLeft ->Translate(glm::vec3( 0.00f, -0.90f, 1.80f));
		cubeRight->Translate(glm::vec3(-0.90f,  0.00f, 1.80f));
		sphrModel->Translate(glm::vec3( 0.00f,  0.00f, 0.50f));

		cubeBase ->Scale(glm::vec3(2.0f, 2.0f, 0.2f));
		cubeLeft ->Scale(glm::vec3(2.0f, 0.2f, 2.0f));
		cubeRight->Scale(glm::vec3(0.2f, 2.0f, 2.0f));

		dirLight.position  = glm::vec3(1.2f, 5.0f, 5.0f);
		spotLight.position = glm::vec3(0.0f, 0.0f, 3.0f);
		spotLight.ambient  = glm::vec3(0.15f);

		shaderManager->CreateDirectionalLight(dirLight);
		shaderManager->CreateSpotLight(spotLight);
		shaderManager->ActivateShadow(false);
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

		// Ui Stuff
		shaderManager->UIModifySpotLight();
		shaderManager->UIModifyDirectionLight();
		shaderManager->UIRenderShadowMap();

		windowUI->UIGizmos();
		
		AddShape();

		ImGui::ShowDemoWindow();
	}

	void RenderScene() const
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//cubeBase ->Draw();
		//cubeLeft ->Draw();
		//cubeRight->Draw();
		//sphrModel->Draw();

		for (auto model : Teapot::Model::s_Models)
		{
			model->Draw();
		}
	}

	void AddShape()
	{
		ImGui::Begin("Add Shape", nullptr, 0);
		if (ImGui::Button("Add Shape"))
		{
			static unsigned int counter = 0;
			Teapot::Model::CreateModel(
				cube1.ShapePositions(),
				cube1.ShapeColors(),
				cube1.ShapeNormals(),
				cube1.ShapeIndices(),
				"Cube" + std::to_string(counter)
			);

			counter++;
		}
		ImGui::End();
	}

private:
	Shapes::Cube cube1{ 1.0f, glm::vec3(0.3f, 0.9f, 1.0f) };
	Shapes::Cube cube2{ 1.0f, glm::vec3(0.3f, 0.9f, 1.0f) };
	Shapes::Cube cube3{ 1.0f, glm::vec3(0.3f, 0.9f, 1.0f) };
	Shapes::Sphere sphere{ 0.30f, glm::vec3(1.0f, 0.87f, 0.0f), 30, 30 };
	
	std::shared_ptr<Teapot::Model> cubeBase;
	std::shared_ptr<Teapot::Model> cubeLeft;
	std::shared_ptr<Teapot::Model> cubeRight;
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
	Teapot::WindowProps windowProps = { "Shape Demo", 1280, 720 };
	auto s = std::make_unique<Sandbox>(windowProps);
	s->GetWindow().GetWidth();
	s->Run();
	s->OnUpdate();
}