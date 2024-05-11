#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Teapot/Shadow.h"
#include "Camera/Camera.h"

#include <imgui.h>
#include <functional>

namespace Teapot
{
	struct DirectionalLight
	{
		glm::vec3 direction{ 0.5f, -0.4f, -0.4f };
		glm::vec3 ambient  { 0.1f };
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
		ShaderManager();
		ShaderManager(ShaderManager& other) = delete;
		void operator=(const ShaderManager&) = delete;
		~ShaderManager();

		void SetShaderValues(Camera& camera);
		void RunShader();

		static ShaderManager* GetInstance();

	// Inline functions
	public:
		inline Teapot::Shader& GetShader() { return m_Shader; }
		inline Teapot::Shader& GetShadowShader() { return m_ShaderDepthBasic; }
		inline void RenderShadow() { m_Shadow->RenderShadow(glm::vec3{0.0f}, Model::models); }

	// Functions
	public:
		void CreateDirectionalLight(DirectionalLight& directionalLight);
		void CreateSpotLight(SpotLight& spotLight);
		void CreatePointLight(PointLight& pointLight);
		
		void UIModifyDirectionLight();
		void UIModifyPointLight();
		void UIModifySpotLight();

	private:
		static ShaderManager* s_ShaderManager;

		Teapot::Shader m_ShaderDepthBasic;
		Teapot::Shader m_Shader;
		Teapot::Shadow* m_Shadow;

		std::vector<PointLight> m_pointLights;
		DirectionalLight* m_DirectionalLight;
		SpotLight* m_SpotLight;

		bool IsDirectionalLightCreated{ false };
		bool IsSpotLightCreated{ false };

		Camera* m_Camera{};
	};
}