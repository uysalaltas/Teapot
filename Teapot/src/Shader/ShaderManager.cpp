#include "ShaderManager.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	std::shared_ptr<ShaderManager> ShaderManager::s_ShaderManager = nullptr;

	std::shared_ptr<ShaderManager> ShaderManager::GetInstance()
	{
		if (s_ShaderManager == nullptr) {
			std::cout << "ShaderManager Created!\n";
			s_ShaderManager = std::make_unique<ShaderManager>();
		}
		return s_ShaderManager;
	}

	void ShaderManager::SetShaderValues(Camera& camera)
	{
		m_Camera = &camera;
	}

	void ShaderManager::RunShader()
	{
		if (!m_Camera)
		{
			std::cout << "Camera not found!\n";
			return;
		}

		m_Shader.Bind();

        m_Shader.SetUniform1i("material.diffuse"  , 0    );
        m_Shader.SetUniform1i("material.specular" , 1    );
		m_Shader.SetUniform1f("material.shininess", 32.0f);

		auto directionalLightCount = m_DirectionalLights.size();
		m_Shader.SetUniform1i("directionalLightCount", directionalLightCount);
		for (int i = 0; i < directionalLightCount; i++)
		{
			m_Shader.SetUniformVec3f(std::format("dirLights[{}].position" , i), m_DirectionalLights[i].position );
			m_Shader.SetUniformVec3f(std::format("dirLights[{}].direction", i), m_DirectionalLights[i].direction);
			m_Shader.SetUniformVec3f(std::format("dirLights[{}].ambient"  , i), m_DirectionalLights[i].ambient  );
			m_Shader.SetUniformVec3f(std::format("dirLights[{}].diffuse"  , i), m_DirectionalLights[i].diffuse  );
			m_Shader.SetUniformVec3f(std::format("dirLights[{}].specular" , i), m_DirectionalLights[i].specular );
		}

		auto pointLightCount = m_pointLights.size();
		m_Shader.SetUniform1i("pointLightCount", pointLightCount);
        for (int i = 0; i < pointLightCount; i++)
        {
            m_Shader.SetUniformVec3f(std::format("pointLights[{}].position", i), m_pointLights[i].position );
            m_Shader.SetUniformVec3f(std::format("pointLights[{}].ambient" , i), m_pointLights[i].ambient  );
            m_Shader.SetUniformVec3f(std::format("pointLights[{}].diffuse" , i), m_pointLights[i].diffuse  );
            m_Shader.SetUniformVec3f(std::format("pointLights[{}].specular", i), m_pointLights[i].specular );
            m_Shader.SetUniform1f(std::format("pointLights[{}].constant" , i), m_pointLights[i].constant );
			m_Shader.SetUniform1f(std::format("pointLights[{}].linear"   , i), m_pointLights[i].linear   );
			m_Shader.SetUniform1f(std::format("pointLights[{}].quadratic", i), m_pointLights[i].quadratic);
        }

		auto spotLightCount = m_SpotLights.size();
		m_Shader.SetUniform1i("spotLightCount", spotLightCount);
		for (int i = 0; i < spotLightCount; i++)
		{
			m_Shader.SetUniformVec3f(std::format("spotLights[{}].position" , i), m_SpotLights[i].position   );
			m_Shader.SetUniformVec3f(std::format("spotLights[{}].direction", i), m_SpotLights[i].direction  );
			m_Shader.SetUniformVec3f(std::format("spotLights[{}].ambient"  , i), m_SpotLights[i].ambient    );
			m_Shader.SetUniformVec3f(std::format("spotLights[{}].diffuse"  , i), m_SpotLights[i].diffuse    );
			m_Shader.SetUniformVec3f(std::format("spotLights[{}].specular" , i), m_SpotLights[i].specular   );
			m_Shader.SetUniform1f(std::format("spotLights[{}].constant"   , i), m_SpotLights[i].constant   );
			m_Shader.SetUniform1f(std::format("spotLights[{}].linear"     , i), m_SpotLights[i].linear     );
			m_Shader.SetUniform1f(std::format("spotLights[{}].quadratic"  , i), m_SpotLights[i].quadratic  );
			m_Shader.SetUniform1f(std::format("spotLights[{}].cutOff"     , i), m_SpotLights[i].cutOff     );
			m_Shader.SetUniform1f(std::format("spotLights[{}].outerCutOff", i), m_SpotLights[i].outerCutOff);
		}

		m_Shader.SetUniformMat4f("view"      , m_Camera->GetViewMatrix());
		m_Shader.SetUniformMat4f("projection", m_Camera->GetProjMatrix());
		m_Shader.SetUniformVec3f("camPos"    , m_Camera->GetEye());

		for (int i = 0; i < m_Shadows.size(); i++)
		{
			m_Shader.SetUniformMat4f(std::format("lightSpaceMatrix[{}]", i), m_Shadows[i]->GetLightSpaceMatrix());
			m_Shadows[i]->BindShadow();
		}
	}

	void ShaderManager::CreateDirectionalLight(const DirectionalLight& directionalLight)
	{
		m_DirectionalLights.push_back(directionalLight);
		m_Shadows.push_back(std::make_unique<Teapot::Shadow>(m_Shader, m_ShaderDepthBasic));
	}

	void ShaderManager::CreateSpotLight(const SpotLight& spotLight)
	{
		m_SpotLights.push_back(spotLight);
		m_Shadows.push_back(std::make_unique<Teapot::Shadow>(m_Shader, m_ShaderDepthBasic));
	}

	void ShaderManager::CreatePointLight(const PointLight& pointLight)
    {
        m_pointLights.push_back(pointLight);
    }

	void ShaderManager::RenderShadow() const
	{
		m_Shadows[0]->RenderShadow(m_DirectionalLights[0].position, Model::s_Models, Shadow::RenderType::Ortho);
		m_Shadows[1]->RenderShadow(m_SpotLights[0].position, Model::s_Models, Shadow::RenderType::Perspective);
	}
	
	void ShaderManager::UIModifyDirectionLight()
	{
		for (int i = 0; i < m_DirectionalLights.size(); i++)
		{
			auto title = std::format("Directional Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position" , &m_DirectionalLights[i].position[0] , -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Direction", &m_DirectionalLights[i].direction[0], -1.0f , 1.0f);
			ImGui::SliderFloat("Ambient ", &m_DirectionalLights[i].ambient[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("Specular", &m_DirectionalLights[i].specular[0], 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse ", &m_DirectionalLights[i].diffuse[0] , 0.0f, 1.0f);
			ImGui::End();

			m_DirectionalLights[i].ambient[1]  = m_DirectionalLights[i].ambient[0];
			m_DirectionalLights[i].ambient[2]  = m_DirectionalLights[i].ambient[0];
			m_DirectionalLights[i].specular[1] = m_DirectionalLights[i].specular[0];
			m_DirectionalLights[i].specular[2] = m_DirectionalLights[i].specular[0];
			m_DirectionalLights[i].diffuse[1]  = m_DirectionalLights[i].diffuse[0];
			m_DirectionalLights[i].diffuse[2]  = m_DirectionalLights[i].diffuse[0];
		}
	}
	
	void ShaderManager::UIModifyPointLight()
	{
		for (int i = 0; i < m_pointLights.size(); i++)
		{
			auto title = std::format("Point Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position", &m_pointLights[i].position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("Ambient " , &m_pointLights[i].ambient[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("Specular ", &m_pointLights[i].specular[0], 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse " , &m_pointLights[i].diffuse[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("constant" , &m_pointLights[i].constant   , 0.0f, 1.0f);
			ImGui::SliderFloat("linear"   , &m_pointLights[i].linear     , 0.0f, 1.0f);
			ImGui::SliderFloat("quadratic", &m_pointLights[i].quadratic  , 0.0f, 1.0f);
			ImGui::End();

			m_pointLights[i].ambient[1]  = m_pointLights[i].ambient[0];
			m_pointLights[i].ambient[2]  = m_pointLights[i].ambient[0];
			m_pointLights[i].specular[1] = m_pointLights[i].specular[0];
			m_pointLights[i].specular[2] = m_pointLights[i].specular[0];
			m_pointLights[i].diffuse[1]  = m_pointLights[i].diffuse[0];
			m_pointLights[i].diffuse[2]  = m_pointLights[i].diffuse[0];
		}
	}

	void ShaderManager::UIModifySpotLight()
	{
		for (int i = 0; i < m_SpotLights.size(); i++)
		{
			auto title = std::format("Spot Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position"  , &m_SpotLights[i].position[0] , -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Direction" , &m_SpotLights[i].direction[0], -1.0f , 1.0f );
			ImGui::SliderFloat("Ambient " , &m_SpotLights[i].ambient[0] , 0.0f, 1.0f );
			ImGui::SliderFloat("Specular ", &m_SpotLights[i].specular[0], 0.0f, 1.0f );
			ImGui::SliderFloat("Diffuse " , &m_SpotLights[i].diffuse[0] , 0.0f, 1.0f );
			ImGui::SliderFloat("constant" , &m_SpotLights[i].constant   , 0.0f, 1.0f );
			ImGui::SliderFloat("linear"   , &m_SpotLights[i].linear     , 0.0f, 1.0f );
			ImGui::SliderFloat("quadratic", &m_SpotLights[i].quadratic  , 0.0f, 1.0f );
			ImGui::End();

			m_SpotLights[i].ambient[1]  = m_SpotLights[i].ambient[0];
			m_SpotLights[i].ambient[2]  = m_SpotLights[i].ambient[0];
			m_SpotLights[i].specular[1] = m_SpotLights[i].specular[0];
			m_SpotLights[i].specular[2] = m_SpotLights[i].specular[0];
			m_SpotLights[i].diffuse[1]  = m_SpotLights[i].diffuse[0];
			m_SpotLights[i].diffuse[2]  = m_SpotLights[i].diffuse[0];
		}
	}

	void ShaderManager::UIRenderShadowMap()
	{
		ImGui::Begin("Debug");
		{
			ImGui::SliderInt("slider int", &m_selectedShadowMap, 0, m_Shadows.size() - 1);

			int minRegion = (ImGui::GetContentRegionAvail().x > ImGui::GetContentRegionAvail().y) 
				? ImGui::GetContentRegionAvail().y : ImGui::GetContentRegionAvail().x;

			ImGui::BeginChild("ShadowMap");
			ImGui::Image(
				(ImTextureID)GetShadowID(),
				ImVec2(minRegion, minRegion),
				ImVec2(0, 1),
				ImVec2(1, 0)
			);
		}
		ImGui::EndChild();
		ImGui::End();

	}
}