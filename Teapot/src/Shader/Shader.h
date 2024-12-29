#pragma once

#include <string>
#include <unordered_map>
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assert.h>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

namespace Teapot
{
	class Shader {
	public:
		explicit Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniformVec3f(const std::string& name, const glm::vec3& vector);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

	private:
		ShaderProgramSource ParseShader(const std::string& filepath) const;
		unsigned int CompileShader(unsigned int type, const std::string& source) const;
		int GetUniformLocation(const std::string& name);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) const;

	private:
		std::string m_Filepath;
		unsigned int m_RendererID{};
		std::unordered_map<std::string, int> m_UniformLocationCache;

	};
}