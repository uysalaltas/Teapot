#pragma once

#include "Shader/Shader.h"
#include "Shadow/Shadow.h"
#include "Scene/SceneContext.h"

#include <imgui.h>
#include <functional>
#include <format>

namespace Teapot
{
	struct DirectionalLight
	{
		glm::vec3 position { 0.0f, 0.0f, 5.0f };
		glm::vec3 direction{ 0.5f, -0.4f, -0.4f };
		glm::vec3 ambient  { 0.3f };
		glm::vec3 diffuse  { 0.3f };
		glm::vec3 specular { 0.1f };
	};

	struct PointLight
	{
		glm::vec3 position{ 0.0f, 0.0f, 1.0f };
		glm::vec3 ambient { 0.05f};
		glm::vec3 diffuse { 0.8f };
		glm::vec3 specular{ 1.0f };
		float constant { 1.0f   };
		float linear   { 0.09f  };
		float quadratic{ 0.032f };
	};

	struct SpotLight
	{
		glm::vec3 position { 0.0f, 0.0f, 5.0f };
		glm::vec3 direction{ 0.0f, -1.0f, 0.0f};
		glm::vec3 ambient  { 0.3f };
		glm::vec3 diffuse  { 0.3f };
		glm::vec3 specular { 0.1f };
		float constant   { 1.0f   };
		float linear     { 0.09f  };
		float quadratic  { 0.032f };
		float cutOff     { glm::cos(glm::radians(12.5f)) };
		float outerCutOff{ glm::cos(glm::radians(15.0f)) };
	};

	class Light
	{
	public:
		Light(Teapot::Shader& shader, Teapot::Shader& shaderDepthBasic);
		void RunShader();

		inline int GetShadowID() const { return m_Shadows[m_selectedShadowMap]->shadowMapping->GetShadowMapTexture(); }
		inline void ActivateShadow(bool activateShadow) { m_activateShadow = activateShadow;  }
		inline bool IsShadowActive() const { return m_activateShadow; }

		inline std::vector<std::unique_ptr<Shadow>>& GetShadows() { return m_Shadows; };

		void CreateDirectionalLight(const DirectionalLight& directionalLight);
		void CreateSpotLight(const SpotLight& spotLight);
		void CreatePointLight(const PointLight& pointLight);
		
		bool RenderShadow();
		
		void UIModifyDirectionLight();
		void UIModifyPointLight();
		void UIModifySpotLight();
		void UIRenderShadowMap();

	private:
		int m_selectedShadowMap{};
		bool m_activateShadow{};

		Teapot::Shader& m_shader;
		Teapot::Shader& m_shaderDepthBasic;

		std::vector<std::unique_ptr<Shadow>> m_Shadows;

		std::vector<DirectionalLight> m_DirectionalLights;
		std::vector<PointLight> m_pointLights;
		std::vector<SpotLight> m_SpotLights;
	};
}