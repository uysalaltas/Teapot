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
			m_Shader.SetUniformVec3f("dirLights["+ std::to_string(i) +"].position" , m_DirectionalLights[i].position );
			m_Shader.SetUniformVec3f("dirLights["+ std::to_string(i) +"].direction", m_DirectionalLights[i].direction);
			m_Shader.SetUniformVec3f("dirLights["+ std::to_string(i) +"].ambient"  , m_DirectionalLights[i].ambient  );
			m_Shader.SetUniformVec3f("dirLights["+ std::to_string(i) +"].diffuse"  , m_DirectionalLights[i].diffuse  );
			m_Shader.SetUniformVec3f("dirLights["+ std::to_string(i) +"].specular" , m_DirectionalLights[i].specular );
		}

		auto pointLightCount = m_pointLights.size();
		m_Shader.SetUniform1i("pointLightCount", pointLightCount);
        for (int i = 0; i < pointLightCount; i++)
        {
            m_Shader.SetUniformVec3f("pointLights["+ std::to_string(i) +"].position", m_pointLights[i].position );
            m_Shader.SetUniformVec3f("pointLights["+ std::to_string(i) +"].ambient" , m_pointLights[i].ambient  );
            m_Shader.SetUniformVec3f("pointLights["+ std::to_string(i) +"].diffuse" , m_pointLights[i].diffuse  );
            m_Shader.SetUniformVec3f("pointLights["+ std::to_string(i) +"].specular", m_pointLights[i].specular );
            m_Shader.SetUniform1f("pointLights[" + std::to_string(i) + "].constant" , m_pointLights[i].constant );
			m_Shader.SetUniform1f("pointLights[" + std::to_string(i) + "].linear"   , m_pointLights[i].linear   );
			m_Shader.SetUniform1f("pointLights[" + std::to_string(i) + "].quadratic", m_pointLights[i].quadratic);
        }

		auto spotLightCount = m_SpotLights.size();
		m_Shader.SetUniform1i("spotLightCount", spotLightCount);
		for (int i = 0; i < spotLightCount; i++)
		{
			m_Shader.SetUniformVec3f("spotLights["+ std::to_string(i) +"].position" , m_SpotLights[i].position   );
			m_Shader.SetUniformVec3f("spotLights["+ std::to_string(i) +"].direction", m_SpotLights[i].direction  );
			m_Shader.SetUniformVec3f("spotLights["+ std::to_string(i) +"].ambient"  , m_SpotLights[i].ambient    );
			m_Shader.SetUniformVec3f("spotLights["+ std::to_string(i) +"].diffuse"  , m_SpotLights[i].diffuse    );
			m_Shader.SetUniformVec3f("spotLights["+ std::to_string(i) +"].specular" , m_SpotLights[i].specular   );
			m_Shader.SetUniform1f("spotLights["+ std::to_string(i) +"].constant"    , m_SpotLights[i].constant   );
			m_Shader.SetUniform1f("spotLights["+ std::to_string(i) +"].linear"      , m_SpotLights[i].linear     );
			m_Shader.SetUniform1f("spotLights["+ std::to_string(i) +"].quadratic"   , m_SpotLights[i].quadratic  );
			m_Shader.SetUniform1f("spotLights["+ std::to_string(i) +"].cutOff"      , m_SpotLights[i].cutOff     );
			m_Shader.SetUniform1f("spotLights["+ std::to_string(i) +"].outerCutOff" , m_SpotLights[i].outerCutOff);
		}

		m_Shader.SetUniformMat4f("view"      , m_Camera->GetViewMatrix());
		m_Shader.SetUniformMat4f("projection", m_Camera->GetProjMatrix());
		m_Shader.SetUniformVec3f("camPos"    , m_Camera->GetEye());

		for (int i = 0; i < m_Shadows.size(); i++)
		{
			m_Shader.SetUniformMat4f("lightSpaceMatrix[" + std::to_string(i) + "]", m_Shadows[i]->GetLightSpaceMatrix());
			m_Shadows[i]->BindShadow();
		}
	}

	void ShaderManager::CreateDirectionalLight(const DirectionalLight& directionalLight)
	{
		m_DirectionalLights.push_back(directionalLight);
		//m_Shadows.push_back(new Teapot::Shadow(m_Shader, m_ShaderDepthBasic));
	}

	void ShaderManager::CreateSpotLight(const SpotLight& spotLight)
	{
		m_SpotLights.push_back(spotLight);
		//m_Shadows.push_back(new Teapot::Shadow(m_Shader, m_ShaderDepthBasic));
	}

	void ShaderManager::CreatePointLight(const PointLight& pointLight)
    {
        m_pointLights.push_back(pointLight);
    }

	void ShaderManager::RenderShadow() const
	{
		//if(IsSpotLightCreated)
		//{
		//	m_Shadows[1]->RenderShadow(m_SpotLight->position, Model::models, Shadow::RenderType::Perspective);
		//}

		//if (IsDirectionalLightCreated)
		//{
		//	m_Shadows[0]->RenderShadow(m_DirectionalLight->position, Model::models, Shadow::RenderType::Ortho);
		//}
	}
	
	void ShaderManager::UIModifyDirectionLight()
	{
		for (int i = 0; i < m_DirectionalLights.size(); i++)
		{
			auto title = "Directional Light " + std::to_string(i);
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
			auto title = "Point Light " + std::to_string(i);
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
			auto title = "Spot Light " + std::to_string(i);
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
}