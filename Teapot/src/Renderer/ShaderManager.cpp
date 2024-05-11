#include "ShaderManager.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	ShaderManager* ShaderManager::s_ShaderManager = nullptr;

	ShaderManager* ShaderManager::GetInstance()
	{
		if (s_ShaderManager == nullptr) {
			std::cout << "ShaderManager Created!\n";
			s_ShaderManager = new ShaderManager();
		}
		return s_ShaderManager;
	}

	ShaderManager::ShaderManager()
		: m_ShaderDepthBasic("../vendor/Teapot/Teapot/shaders/BasicDepth.shader")
		, m_Shader("../vendor/Teapot/Teapot/shaders/MaterialShader.shader")
	{
		m_Shadow = new Teapot::Shadow(m_Shader, m_ShaderDepthBasic);
	}

	ShaderManager::~ShaderManager()
	{
		delete m_Shadow;
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

		if (IsDirectionalLightCreated)
		{
			m_Shader.SetUniform1i("DirLightActive", 1);
			// directional light
			m_Shader.SetUniformVec3f("dirLight.direction", m_DirectionalLight->direction);
			m_Shader.SetUniformVec3f("dirLight.ambient"  , m_DirectionalLight->ambient  );
			m_Shader.SetUniformVec3f("dirLight.diffuse"  , m_DirectionalLight->diffuse  );
			m_Shader.SetUniformVec3f("dirLight.specular" , m_DirectionalLight->specular );
		}
		else
		{
			m_Shader.SetUniform1i("DirLightActive", 0);
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

		if (IsSpotLightCreated)
		{
			m_Shader.SetUniform1i("SpotLightActive", 1);
			// spotLight
			m_Shader.SetUniformVec3f("spotLight.position" , m_SpotLight->position   );
			m_Shader.SetUniformVec3f("spotLight.direction", m_SpotLight->direction  );
			m_Shader.SetUniformVec3f("spotLight.ambient"  , m_SpotLight->ambient    );
			m_Shader.SetUniformVec3f("spotLight.diffuse"  , m_SpotLight->diffuse    );
			m_Shader.SetUniformVec3f("spotLight.specular" , m_SpotLight->specular   );
			m_Shader.SetUniform1f("spotLight.constant"    , m_SpotLight->constant   );
			m_Shader.SetUniform1f("spotLight.linear"      , m_SpotLight->linear     );
			m_Shader.SetUniform1f("spotLight.quadratic"   , m_SpotLight->quadratic  );
			m_Shader.SetUniform1f("spotLight.cutOff"      , m_SpotLight->cutOff     );
			m_Shader.SetUniform1f("spotLight.outerCutOff" , m_SpotLight->outerCutOff);
		}
		else
		{
			m_Shader.SetUniform1i("SpotLightActive", 0);
		}

		m_Shader.SetUniformMat4f("view"            , m_Camera->GetViewMatrix());
		m_Shader.SetUniformMat4f("projection"      , m_Camera->GetProjMatrix());
		m_Shader.SetUniformMat4f("lightSpaceMatrix", m_Shadow->GetLightSpaceMatrix());
		m_Shader.SetUniformVec3f("camPos"          , m_Camera->GetEye());

		m_Shadow->BindShadow();
	}

	void ShaderManager::CreateDirectionalLight(DirectionalLight& directionalLight)
	{
		IsDirectionalLightCreated = true;
		m_DirectionalLight = &directionalLight;
	}

	void ShaderManager::CreateSpotLight(SpotLight& spotLight)
	{
		IsSpotLightCreated = true;
		m_SpotLight = &spotLight;
	}

	void ShaderManager::CreatePointLight(PointLight& pointLight)
    {
        m_pointLights.push_back(pointLight);
    }
	
	void ShaderManager::UIModifyDirectionLight()
	{
		ImGui::Begin("Direction Light", NULL, 0);
		ImGui::SliderFloat3("Light Direction", &m_DirectionalLight->direction[0], 0.0f, 1.0f);
		ImGui::SliderFloat("Ambient " , &m_DirectionalLight->ambient[0] , 0.0f, 1.0f);
		ImGui::SliderFloat("Specular" , &m_DirectionalLight->specular[0], 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse " , &m_DirectionalLight->diffuse[0] , 0.0f, 1.0f);
		ImGui::End();

		m_DirectionalLight->ambient[1]  = m_DirectionalLight->ambient[0];
		m_DirectionalLight->ambient[2]  = m_DirectionalLight->ambient[0];
		m_DirectionalLight->specular[1] = m_DirectionalLight->specular[0];
		m_DirectionalLight->specular[2] = m_DirectionalLight->specular[0];
		m_DirectionalLight->diffuse[1]  = m_DirectionalLight->diffuse[0];
		m_DirectionalLight->diffuse[2]  = m_DirectionalLight->diffuse[0];
	}
	
	void ShaderManager::UIModifyPointLight()
	{
		for (int i = 0; i < m_pointLights.size(); i++)
		{
			auto title = "Point Light " + std::to_string(i);
			ImGui::Begin(title.c_str(), NULL, 0);
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
		ImGui::Begin("Spot Light", NULL, 0);
		ImGui::SliderFloat3("Light Position"  , &m_SpotLight->position[0] , -10.0f, 10.0f);
		ImGui::SliderFloat3("Light Direction" , &m_SpotLight->direction[0],  0.0f , 1.0f );
		ImGui::SliderFloat("Ambient " , &m_SpotLight->ambient[0] , 0.0f , 1.0f );
		ImGui::SliderFloat("Specular ", &m_SpotLight->specular[0], 0.0f , 1.0f );
		ImGui::SliderFloat("Diffuse " , &m_SpotLight->diffuse[0] , 0.0f , 1.0f );
		ImGui::SliderFloat("constant" , &m_SpotLight->constant   , 0.0f , 1.0f );
		ImGui::SliderFloat("linear"   , &m_SpotLight->linear     , 0.0f , 1.0f );
		ImGui::SliderFloat("quadratic", &m_SpotLight->quadratic  , 0.0f , 1.0f );
		ImGui::End();

		m_SpotLight->ambient[1]  = m_SpotLight->ambient[0];
		m_SpotLight->ambient[2]  = m_SpotLight->ambient[0];
		m_SpotLight->specular[1] = m_SpotLight->specular[0];
		m_SpotLight->specular[2] = m_SpotLight->specular[0];
		m_SpotLight->diffuse[1]  = m_SpotLight->diffuse[0];
		m_SpotLight->diffuse[2]  = m_SpotLight->diffuse[0];
	}
}