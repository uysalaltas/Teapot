#include "Light.h"
#include <glm/gtx/string_cast.hpp>

namespace Teapot
{
	Light::Light(Teapot::Shader& shader, Teapot::Shader& shaderDepthBasic)
		: m_shader(shader)
		, m_shaderDepthBasic(shaderDepthBasic)
	{

	}

	void Light::RunShader()
	{
		m_shader.Bind();

		m_shader.SetUniform1i("material.diffuse"  , 0    );
		m_shader.SetUniform1i("material.specular" , 1    );
		m_shader.SetUniform1f("material.shininess", 32.0f);

		auto directionalLightCount = mv_directionalLights.size();
		m_shader.SetUniform1i("directionalLightCount", directionalLightCount);
		for (int i = 0; i < directionalLightCount; i++)
		{
			m_shader.SetUniformVec3f(std::format("dirLights[{}].position" , i), mv_directionalLights[i].position );
			m_shader.SetUniformVec3f(std::format("dirLights[{}].direction", i), mv_directionalLights[i].direction);
			m_shader.SetUniformVec3f(std::format("dirLights[{}].ambient"  , i), mv_directionalLights[i].ambient  );
			m_shader.SetUniformVec3f(std::format("dirLights[{}].diffuse"  , i), mv_directionalLights[i].diffuse  );
			m_shader.SetUniformVec3f(std::format("dirLights[{}].specular" , i), mv_directionalLights[i].specular );
		}

		auto pointLightCount = mv_pointLights.size();
		m_shader.SetUniform1i("pointLightCount", pointLightCount);
        for (int i = 0; i < pointLightCount; i++)
        {
			m_shader.SetUniformVec3f(std::format("pointLights[{}].position", i), mv_pointLights[i].position );
			m_shader.SetUniformVec3f(std::format("pointLights[{}].ambient" , i), mv_pointLights[i].ambient  );
			m_shader.SetUniformVec3f(std::format("pointLights[{}].diffuse" , i), mv_pointLights[i].diffuse  );
			m_shader.SetUniformVec3f(std::format("pointLights[{}].specular", i), mv_pointLights[i].specular );
			m_shader.SetUniform1f(std::format("pointLights[{}].constant" , i), mv_pointLights[i].constant );
			m_shader.SetUniform1f(std::format("pointLights[{}].linear"   , i), mv_pointLights[i].linear   );
			m_shader.SetUniform1f(std::format("pointLights[{}].quadratic", i), mv_pointLights[i].quadratic);
        }

		auto spotLightCount = mv_spotLights.size();
		m_shader.SetUniform1i("spotLightCount", spotLightCount);
		for (int i = 0; i < spotLightCount; i++)
		{
			m_shader.SetUniformVec3f(std::format("spotLights[{}].position" , i), mv_spotLights[i].position   );
			m_shader.SetUniformVec3f(std::format("spotLights[{}].direction", i), mv_spotLights[i].direction  );
			m_shader.SetUniformVec3f(std::format("spotLights[{}].ambient"  , i), mv_spotLights[i].ambient    );
			m_shader.SetUniformVec3f(std::format("spotLights[{}].diffuse"  , i), mv_spotLights[i].diffuse    );
			m_shader.SetUniformVec3f(std::format("spotLights[{}].specular" , i), mv_spotLights[i].specular   );
			m_shader.SetUniform1f(std::format("spotLights[{}].constant"   , i), mv_spotLights[i].constant   );
			m_shader.SetUniform1f(std::format("spotLights[{}].linear"     , i), mv_spotLights[i].linear     );
			m_shader.SetUniform1f(std::format("spotLights[{}].quadratic"  , i), mv_spotLights[i].quadratic  );
			m_shader.SetUniform1f(std::format("spotLights[{}].cutOff"     , i), mv_spotLights[i].cutOff     );
			m_shader.SetUniform1f(std::format("spotLights[{}].outerCutOff", i), mv_spotLights[i].outerCutOff);
		}

		m_shader.SetUniformMat4f("view"      , Teapot::SceneContext::Get().GetCamera().GetViewMatrix());
		m_shader.SetUniformMat4f("projection", Teapot::SceneContext::Get().GetCamera().GetProjMatrix());
		m_shader.SetUniformVec3f("camPos"    , Teapot::SceneContext::Get().GetCamera().GetEye());

		for (int i = 0; i < mv_shadows.size(); i++)
		{
			m_shader.SetUniformMat4f(std::format("lightSpaceMatrix[{}]", i), mv_shadows[i]->GetLightSpaceMatrix());
			mv_shadows[i]->BindShadow();
		}
	}

	void Light::CreateDirectionalLight(const DirectionalLight& directionalLight)
	{
		mv_directionalLights.push_back(directionalLight);
		mv_shadows.push_back(std::make_unique<Teapot::Shadow>(m_shader, m_shaderDepthBasic, mv_directionalLights.back().position, Shadow::RenderType::Ortho));
	}

	void Light::CreateSpotLight(const SpotLight& spotLight)
	{
		mv_spotLights.push_back(spotLight);
		mv_shadows.push_back(std::make_unique<Teapot::Shadow>(m_shader, m_shaderDepthBasic, mv_spotLights.back().position, Shadow::RenderType::Perspective));
	}

	void Light::CreatePointLight(const PointLight& pointLight)
    {
        mv_pointLights.push_back(pointLight);
    }

	bool Light::RenderShadow()
	{
		m_shader.Bind();
		if (m_activateShadow)
		{
			m_shader.SetUniform1i("activateShadow", 1);
			return true;
		}
		else
		{
			m_shader.SetUniform1i("activateShadow", 0);
			return false;
		}
	}
	
	void Light::UIModifyDirectionLight()
	{
		for (int i = 0; i < mv_directionalLights.size(); i++)
		{
			auto title = std::format("Directional Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position" , &mv_directionalLights[i].position[0] , -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Direction", &mv_directionalLights[i].direction[0], -1.0f , 1.0f);
			ImGui::SliderFloat("Ambient ", &mv_directionalLights[i].ambient[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("Specular", &mv_directionalLights[i].specular[0], 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse ", &mv_directionalLights[i].diffuse[0] , 0.0f, 1.0f);
			ImGui::End();

			mv_directionalLights[i].ambient[1]  = mv_directionalLights[i].ambient[0];
			mv_directionalLights[i].ambient[2]  = mv_directionalLights[i].ambient[0];
			mv_directionalLights[i].specular[1] = mv_directionalLights[i].specular[0];
			mv_directionalLights[i].specular[2] = mv_directionalLights[i].specular[0];
			mv_directionalLights[i].diffuse[1]  = mv_directionalLights[i].diffuse[0];
			mv_directionalLights[i].diffuse[2]  = mv_directionalLights[i].diffuse[0];
		}
	}
	
	void Light::UIModifyPointLight()
	{
		for (int i = 0; i < mv_pointLights.size(); i++)
		{
			auto title = std::format("Point Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position", &mv_pointLights[i].position[0], -10.0f, 10.0f);
			ImGui::SliderFloat("Ambient " , &mv_pointLights[i].ambient[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("Specular ", &mv_pointLights[i].specular[0], 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse " , &mv_pointLights[i].diffuse[0] , 0.0f, 1.0f);
			ImGui::SliderFloat("constant" , &mv_pointLights[i].constant   , 0.0f, 1.0f);
			ImGui::SliderFloat("linear"   , &mv_pointLights[i].linear     , 0.0f, 1.0f);
			ImGui::SliderFloat("quadratic", &mv_pointLights[i].quadratic  , 0.0f, 1.0f);
			ImGui::End();

			mv_pointLights[i].ambient[1]  = mv_pointLights[i].ambient[0];
			mv_pointLights[i].ambient[2]  = mv_pointLights[i].ambient[0];
			mv_pointLights[i].specular[1] = mv_pointLights[i].specular[0];
			mv_pointLights[i].specular[2] = mv_pointLights[i].specular[0];
			mv_pointLights[i].diffuse[1]  = mv_pointLights[i].diffuse[0];
			mv_pointLights[i].diffuse[2]  = mv_pointLights[i].diffuse[0];
		}
	}

	void Light::UIModifySpotLight()
	{
		for (int i = 0; i < mv_spotLights.size(); i++)
		{
			auto title = std::format("Spot Light {}", i);
			ImGui::Begin(title.c_str(), nullptr, 0);
			ImGui::SliderFloat3("Light Position"  , &mv_spotLights[i].position[0] , -10.0f, 10.0f);
			ImGui::SliderFloat3("Light Direction" , &mv_spotLights[i].direction[0], -1.0f , 1.0f );
			ImGui::SliderFloat("Ambient " , &mv_spotLights[i].ambient[0] , 0.0f, 1.0f );
			ImGui::SliderFloat("Specular ", &mv_spotLights[i].specular[0], 0.0f, 1.0f );
			ImGui::SliderFloat("Diffuse " , &mv_spotLights[i].diffuse[0] , 0.0f, 1.0f );
			ImGui::SliderFloat("constant" , &mv_spotLights[i].constant   , 0.0f, 1.0f );
			ImGui::SliderFloat("linear"   , &mv_spotLights[i].linear     , 0.0f, 1.0f );
			ImGui::SliderFloat("quadratic", &mv_spotLights[i].quadratic  , 0.0f, 1.0f );
			ImGui::End();

			mv_spotLights[i].ambient[1]  = mv_spotLights[i].ambient[0];
			mv_spotLights[i].ambient[2]  = mv_spotLights[i].ambient[0];
			mv_spotLights[i].specular[1] = mv_spotLights[i].specular[0];
			mv_spotLights[i].specular[2] = mv_spotLights[i].specular[0];
			mv_spotLights[i].diffuse[1]  = mv_spotLights[i].diffuse[0];
			mv_spotLights[i].diffuse[2]  = mv_spotLights[i].diffuse[0];
		}
	}

	void Light::UIRenderShadowMap()
	{
		ImGui::Begin("Shadow Map Debug");
		{
			ImGui::SliderInt("Select Shadow Map", &m_selectedShadowMap, 0, mv_shadows.size() - 1);

			auto contentRegion = ImGui::GetContentRegionAvail();
			int minRegion = (contentRegion.x > contentRegion.y) 
				? contentRegion.y : contentRegion.x;

			ImGui::BeginChild("ShadowMap");
			ImGui::Image(
				(ImTextureID)GetShadowID(),
				ImVec2(minRegion, minRegion),
				ImVec2(0, 1), // Top-left corner of the texture
				ImVec2(1, 0)  // Bottom-right corner of the texture
			);
		}
		ImGui::EndChild();
		ImGui::End();

	}
}