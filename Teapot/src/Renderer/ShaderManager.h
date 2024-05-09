#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Teapot/Shadow.h"
#include "Camera/Camera.h"

namespace Teapot
{
	class ShaderManager
	{
	public:
		ShaderManager();
		ShaderManager(ShaderManager& other) = delete;
		void operator=(const ShaderManager&) = delete;

		~ShaderManager();

		void RunShader();
		void SetShaderValues(Camera& camera);

		inline Teapot::Shader& GetShader() { return m_Shader; }
		inline Teapot::Shader& GetShadowShader() { return m_ShaderDepthBasic; }
		inline glm::vec3& GetLightPos() { return lightPos; }

		inline void RenderShadow(){ m_Shadow->RenderShadow(lightPos, Model::models); }

		static ShaderManager* GetInstance();

	public:
		glm::vec3 lightPos{1.0f, 1.0f, 5.0f};

	private:
		Teapot::Shader m_ShaderDepthBasic;
		Teapot::Shader m_Shader;
		Teapot::Shadow* m_Shadow;

		Camera* m_Camera{};

		static ShaderManager* s_ShaderManager;

	};
}