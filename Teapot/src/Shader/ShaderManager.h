#pragma once

#include "Shader.h"
#include "Renderer/Model.h"
#include "Shadow/Shadow.h"
#include "Camera/Camera.h"

#include <imgui.h>
#include <functional>

namespace Teapot
{
	struct DirectionalLight
	{
		glm::vec3 position { 0.0f, 0.0f, 5.0f };
		glm::vec3 direction{ 0.5f, -0.4f, -0.4f };
		glm::vec3 ambient  { 0.5f };
		glm::vec3 diffuse  { 0.4f };
		glm::vec3 specular { 0.5f };
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
		glm::vec3 direction{ 0.0f, 0.0f,-1.0f};
		glm::vec3 ambient  { 1.0f };
		glm::vec3 diffuse  { 1.0f };
		glm::vec3 specular { 1.0f };
		float constant   { 1.0f   };
		float linear     { 0.09f  };
		float quadratic  { 0.032f };
		float cutOff     { glm::cos(glm::radians(12.5f)) };
		float outerCutOff{ glm::cos(glm::radians(15.0f)) };
	};

	class ShaderManager
	{
	// Core functions
	public:
		ShaderManager() = default;

		void SetShaderValues(Camera& camera);
		void RunShader();

		static std::shared_ptr<ShaderManager> GetInstance();

	// Inline functions
	public:
		inline Teapot::Shader& GetShader() { return m_Shader; }
		inline Teapot::Shader& GetShadowShader() { return m_ShaderDepthBasic; }

	// Functions
	public:
		void CreateDirectionalLight(const DirectionalLight& directionalLight);
		void CreateSpotLight(const SpotLight& spotLight);
		void CreatePointLight(const PointLight& pointLight);
		
		void RenderShadow() const;
		
		void UIModifyDirectionLight();
		void UIModifyPointLight();
		void UIModifySpotLight();

	private:
		static std::shared_ptr<ShaderManager> s_ShaderManager;

		Teapot::Shader m_ShaderDepthBasic{"../vendor/Teapot/Teapot/shaders/BasicDepth.shader"};
		Teapot::Shader m_Shader { "../vendor/Teapot/Teapot/shaders/MaterialShader.shader" };;

		std::vector<std::unique_ptr<Shadow>> m_Shadows;

		std::vector<DirectionalLight> m_DirectionalLights;
		std::vector<PointLight> m_pointLights;
		std::vector<SpotLight> m_SpotLights;

		Camera* m_Camera{};
	};
}