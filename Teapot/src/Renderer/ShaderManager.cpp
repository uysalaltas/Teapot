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
		, m_Shader("../vendor/Teapot/Teapot/shaders/Basic.shader")
	{
		m_Shadow = new Teapot::Shadow(m_Shader, m_ShaderDepthBasic);
	}

	ShaderManager::~ShaderManager()
	{
		delete m_Shadow;
	}

	void ShaderManager::RunShader()
	{
		m_Shader.Bind();
		m_Shader.SetUniformMat4f("view"            , m_Camera->GetViewMatrix());
		m_Shader.SetUniformMat4f("projection"      , m_Camera->GetProjMatrix());
		m_Shader.SetUniformMat4f("lightSpaceMatrix", m_Shadow->GetLightSpaceMatrix());
		m_Shader.SetUniformVec3f("camPos"          , m_Camera->GetEye());
		m_Shader.SetUniformVec3f("lightPos"        , lightPos);

		m_Shadow->BindShadow();
	}

	void ShaderManager::SetShaderValues(Camera& camera)
	{
		m_Camera = &camera;
	}

}